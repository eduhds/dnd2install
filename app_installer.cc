#include <iostream>
#include <filesystem>
#include "webview.h"
#include "app_installer.hh"

using namespace std;

int install_command(string path)
{
  filesystem::path filepath(path);
  string filename = filepath.stem().string();
  filesystem::path filepath_without_double_ext(filename);
  string filename_without_ext = filepath_without_double_ext.stem().string();

  string tmp_dir = "/tmp/" + filename_without_ext;

  string mkdir_cmd = "mkdir -p " + tmp_dir;

  string tar_cmd = "tar -xf " + path + " -C " + tmp_dir;

  string mv_cmd = "if [ $(ls -1Ua " + tmp_dir + " | wc -l) -eq 3 ] && [ -d " + tmp_dir + "/* ]; then mv " + tmp_dir + "/* ; else mv " + tmp_dir + "; fi";

  return system((mv_cmd + " && " + tar_cmd + " && " + mv_cmd).c_str());
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
    string path = string(argv[2]);

    install_command(path);

    return 0;
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
