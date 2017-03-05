#include "libraries.hpp"
#include "app-window.hpp"
#include "scroll-pane.hpp"
#include "dialog.hpp"
#include "button.hpp"
#include "edit.hpp"
#include "list-box.hpp"
#include "status-bar.hpp"
#include "toolbar.hpp"
#include "rebar.hpp"
#include "message-pump.hpp"
#include "resource.h"

#include <iostream>
#include <memory>

using namespace std;

void go() {
  INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_BAR_CLASSES
  };
  if (!InitCommonControlsEx(&icce)) {
    assert(false);
  }

  AppWindow w;
  Rebar r(w);
  Toolbar t(r);
  Toolbar t2(r);

  w.Menu(IDR_MENU1);
  w.On(Close, []() {
    PostQuitMessage(0);
  });
  w.On(Command, ID_FILE_EXIT, []() {
    PostQuitMessage(0);
  });

  w.ClientSize(Dimension(600, 400));
  
  t.AddStandardBitmap(IDB_STD_SMALL_COLOR);
  t2.AddStandardBitmap(IDB_STD_SMALL_COLOR);

  t.AddButton(0, STD_FILENEW, 1001, L"New");
  t.AddButton(0, STD_FILEOPEN, 1002, L"Open");
  t.AddButton(0, STD_FILESAVE, 1002, L"Save");

  t2.AddButton(0, STD_COPY, 1001, L"Copy");
  t2.AddButton(0, STD_CUT, 1002, L"Cut");
  t2.AddButton(0, STD_PASTE, 1002, L"Paste");

  t.Autosize();
  t2.Autosize();
  t.Visible(true);
  t2.Visible(true);

  r.AddBand(t);
  r.AddBand(t2);

  w.Visible(true);

  DefaultPump().Pump();
}

int main() {

  go();
  return 0;
}
