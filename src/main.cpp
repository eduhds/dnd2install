/**
 * Dnd2Install
 * A drag and drop installer for Linux.
 * Created by Eduardo H. da Silva on 15/11/2023
 */

#include <iostream>
#include <filesystem>
#include "../libs/webview.h"
#include "../libs/argparse.hpp"
#include "dnd2install.hpp"
#include "util.hpp"

#define PROGRAM_NAME "dnd2install"
#define PROGRAM_VERSION "1.0.0"
#define INSTALL_DIR_ENV "INSTALL_DIR"

using namespace std;

target_file resolveFile(string path);
int installCommand(target_file file);

string currentExtension = "";
string installDir = "/opt";

int main(int argc, char *argv[])
{
  argparse::ArgumentParser program(PROGRAM_NAME, PROGRAM_VERSION);

  program.add_argument("file")
      .help("Tarball, RPM or DEB package file path")
      .required();

  string path;

  try
  {
    program.parse_args(argc, argv);

    path = program.get<string>("file");

    if (!filesystem::exists(path) || filesystem::is_directory(path))
    {
      throw runtime_error("File don't exists: " + path + ".\n");
    }
  }
  catch (const exception &err)
  {
    cerr << err.what() << endl;
    cerr << program;
    exit(1);
  }

  target_file file = resolveFile(path);
  currentExtension = file.extension;

  bool is_valid_file = false;

  for (string ext : allowedExtensions)
  {
    if (file.extension == ext)
    {
      is_valid_file = true;
      break;
    }
  }

  if (!is_valid_file)
  {
    cout << "Invalid file." << endl;
    return 1;
  }

  if (getenv(INSTALL_DIR_ENV) != NULL)
  {
    installDir = getenv(INSTALL_DIR_ENV);
  }

  webview::webview w(false, nullptr);
  w.set_title("Drag and drop to install");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);

  auto bindInstall = [&](const string &seq, const string &req, void * /*arg*/)
  {
    auto installThread = [&, seq, req]()
    {
      int status = installCommand(file);

      if (status == 0)
      {
        string msg = currentExtension != ".deb" && currentExtension != ".rpm" ? " in " + installDir + "/" + file.name : "";
        notifySend("Successfully installed" + msg);
      }
      else
      {
        notifySend("Failed to install");
      }

      string result = "{\"path\":\"" + path + "\",\"status\":" + to_string(status) + "}";

      w.resolve(seq, status, result);
    };

    thread(installThread).detach();
  };

  w.bind("install", bindInstall, nullptr);

  w.set_html(html);
  w.run();

  return 0;
}

target_file resolveFile(string path)
{
  target_file file;

  file.path = path;
  file.basename = filesystem::path(path).stem().string();
  file.extension = filesystem::path(path).extension().string();
  file.name = filesystem::path(file.basename).stem().string();

  return file;
}

int installCommand(target_file file)
{
  string installCmd = "dnd2install-launcher";

  if (file.extension == ".deb")
  {
    installCmd += " deb";
  }
  else if (file.extension == ".rpm")
  {
    installCmd += " rpm";
  }
  else if (file.extension == ".zip")
  {
    installCmd += " zip";
  }
  else
  {
    installCmd += " tar";
  }

  installCmd += " " + file.path;
  installCmd += " " + file.name;
  installCmd += " " + installDir;

  return system(installCmd.c_str());
}
