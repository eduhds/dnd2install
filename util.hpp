#include <iostream>

using namespace std;

void notifySend(string message)
{
  string msg = "notify-send '" + message + "'";
  system(msg.c_str());
}
