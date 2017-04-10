#include "message-pump.hpp"
#include "dialog.hpp"
#include "resource.h"

#include "button-tests.hpp"
#include "edit-tests.hpp"
#include "list-tests.hpp"
#include "scroll-pane-tests.hpp"
#include "status-bar-tests.hpp"
#include "track-bar-tests.hpp"

#include <iostream>
#include <memory>

using namespace std;
using namespace jwt;

int Start() {
  Dialog d(IDD_HOMEPAGE);

  d.On(Close, []() { PostQuitMessage(0); });
  d.On(Command, IDC_BUTTONTESTS, ShowButtonTests);
  d.On(Command, IDC_EDITTESTS, ShowEditTests);
  d.On(Command, IDC_LISTTESTS, ShowListTests);
  d.On(Command, IDC_SCROLLPANETESTS, ShowScrollPaneTests);
  d.On(Command, IDC_STATUSBARTESTS, ShowStatusBarTests);
  d.On(Command, IDC_TRACKANDPROGRESSTESTS, ShowTrackBarTests);

  SetVisible(d, true);

  return DefaultPump().Pump();
}

int main() {
  return Start();
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return Start();
}