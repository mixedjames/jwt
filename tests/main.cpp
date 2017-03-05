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

void go() {
  INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_BAR_CLASSES
  };
  if (!InitCommonControlsEx(&icce)) {
    assert(false);
  }

}

int main() {

  go();
  return 0;
}
