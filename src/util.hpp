#include <iostream>

using namespace std;

struct target_file
{
  string path;
  string name;
  string basename;
  string extension;
};

void notifySend(string message)
{
  string msg = "notify-send '" + message + "'";
  system(msg.c_str());
}
