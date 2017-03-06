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

#include <iostream>
#include <memory>

using namespace std;
using namespace jwt;

void go() {
  AppWindow w;

  Text(w, L"Hello World!");
  ClientSize(w, Dimension(600, 400));

  w.On(Close, []() {
    PostQuitMessage(0);
  });

  Visible(w, true);
  DefaultPump().Pump();
}

int main() {
  go();
  return 0;
}
