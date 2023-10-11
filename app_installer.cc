#include <iostream>
#include <filesystem>
#include "webview.h"
#include "app_installer.hh"

using namespace std;

int install_command(string path)
{
  filesystem::path filepath(path);
  string filename = filepath.stem().string();
  string fileextension = filepath.extension().string();
  filesystem::path filepath_without_double_ext(filename);
  string filename_without_ext = filepath_without_double_ext.stem().string();

  string tmp_dir = "/tmp/" + filename_without_ext;
  string mkdir_cmd = "rm -rf " + tmp_dir + " && mkdir -p " + tmp_dir;

  string tar_cmd = "tar -xf " + path + " -C " + tmp_dir;
  string zip_cmd = "unzip " + path + " -d " + tmp_dir;
  string deb_cmd = "apt install -y " + path;
  string rpm_cmd = "rpm --force -i " + path;

  string install_cmd = tar_cmd;

  if (fileextension == ".deb")
  {
    return system(deb_cmd.c_str());
  }
  else if (fileextension == ".rpm")
  {
    return system(rpm_cmd.c_str());
  }

  if (fileextension == ".zip")
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
  )";

  return system((mkdir_cmd + " && " + install_cmd + " && " + cp_cmd).c_str());
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cout << "Usage: " << argv[0] << " <string>" << endl;
    return 1;
  }

  if (argc == 3)
  {
    if (string(argv[1]) == "install")
    {
      string path = string(argv[2]);

      return install_command(path);
    }
    return 2;
  }

  if (argc > 3)
  {
    return 3;
  }

  string path = string(argv[1]);

  webview::webview w(false, nullptr);
  w.set_title("Drag and drop to install");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);

  w.bind(
      "install",
      [&](const string &seq, const string &req, void * /*arg*/)
      {
        thread([&, seq, req]
               {
                  int status = system(command_as_root("app_installer install " + path).c_str());
                  string result = "{\"path\":\"" + path + "\",\"status\":" + to_string(status) + "}";
                  w.resolve(seq, status,  result); })
            .detach();
      },
      nullptr);

  w.set_html(html);
  w.run();

  return 0;
}
