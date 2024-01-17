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

      if (status)
      {
        string msg = currentExtension != ".deb" && currentExtension != ".rpm" ? " in " + installDir : "";
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
  string tmp_dir = "/tmp/" + file.name;
  string mkdir_cmd = "rm -rf " + tmp_dir + " && mkdir -p " + tmp_dir;

  string tar_cmd = "tar -xf " + file.path + " -C " + tmp_dir;
  string zip_cmd = "unzip " + file.path + " -d " + tmp_dir;
  string deb_cmd = "apt install -y " + file.path;
  string rpm_cmd = "rpm --force -i " + file.path;

  string install_cmd = tar_cmd;

  if (file.extension == ".deb")
  {
    return system(deb_cmd.c_str());
  }
  else if (file.extension == ".rpm")
  {
    return system(rpm_cmd.c_str());
  }

  if (file.extension == ".zip")
  {
    install_cmd = zip_cmd;
  }

  string cp_cmd = "tmp_dir=" + tmp_dir;
  cp_cmd += "; if [ $(ls -1Ua $tmp_dir | wc -l) -eq 3 ] && [ -d $tmp_dir/* ]";
  cp_cmd += "; then cp -r $tmp_dir/* " + installDir; // Avoid unucessary subdirectory
  cp_cmd += "; else cp -r $tmp_dir " + installDir + "; fi";

  return system((mkdir_cmd + " && " + install_cmd + " && " + cp_cmd).c_str());
}
