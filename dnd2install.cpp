#include <iostream>
#include <filesystem>
#include "webview.h"
#include "dnd2install.hpp"

using namespace std;

struct target_file
{
  string path;
  string name;
  string basename;
  string extension;
};

target_file resolve_file(string path);
int install_command(target_file file);

string allowed_file_extensions[6] = {".deb", ".rpm", ".zip", ".tar", ".gz", ".tgz"};

int main(int argc, char *argv[])
{
  string program = string(argv[0]);

  if (argc < 2 || argc > 3)
  {
    cout << "Usage: " << program << " <file_path>" << endl;
    return 1;
  }

  string path = string(string(argv[1]) == "-i" ? argv[2] : argv[1]);

  if (!filesystem::exists(path))
  {
    cout << "File don't exists." << endl;
    return 2;
  }

  if (argc == 3)
  {
    target_file file = resolve_file(path);

    bool is_valid_file = false;

    for (string ext : allowed_file_extensions)
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
      return 3;
    }

    int install_result = install_command(file);

    cout << "Install result: " << install_result << endl;
    return install_result;
  }

  webview::webview w(false, nullptr);
  w.set_title("Drag and drop to install");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);

  auto bindInstall = [&](const string &seq, const string &req, void * /*arg*/)
  {
    thread([&, seq, req]
           {
                  int status = system(command_as_root(program + " -i " + path).c_str());
                     
                  system(status == 0 ? "notify-send 'Successfully installed'" : "notify-send 'Failed to install'");
     
                  string result = "{\"path\":\"" + path + "\",\"status\":" + to_string(status) + "}";

                  w.resolve(seq, status,  result); })
        .detach();
  };

  w.bind("install", bindInstall, nullptr);

  w.set_html(html);
  w.run();

  return 0;
}

target_file resolve_file(string path)
{
  target_file file;

  file.path = path;
  file.basename = filesystem::path(path).stem().string();
  file.extension = filesystem::path(path).extension().string();
  file.name = filesystem::path(file.basename).stem().string();

  return file;
}

int install_command(target_file file)
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

  string cp_cmd = R"(
    tmp_dir=)" + tmp_dir +
                  R"(
    if [ $(ls -1Ua $tmp_dir | wc -l) -eq 3 ] && [ -d $tmp_dir/* ]; then
      cp -r $tmp_dir/* /opt
    else
      cp -r $tmp_dir /opt
    fi

    if [ $? -eq 0 ]; then xdg-open /opt || true; fi
  )";

  return system((mkdir_cmd + " && " + install_cmd + " && " + cp_cmd).c_str());
}
