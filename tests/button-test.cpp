#include "jwt.hpp"
#include "resource.h"

using namespace jwt;

namespace {
  struct ButtonTests {
    Dialog d;
    Button commandBtn;
    SplitButton splitBtn;
    HMENU menu;

    ButtonTests()
      : d(IDD_BUTTONTESTS), commandBtn(d, IDC_COMMANDBUTTON), splitBtn(d, IDC_SPLITBUTTON),
      menu(LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_BUTTONMENU)))
    {
      menu = GetSubMenu(menu, 0);

      d.On(Close, [this]() {
        SetVisible(d, false);
      });

      SetNote(commandBtn, L"Command buttons can have explanatory text underneath their main heading");
      SetIcon(commandBtn, IDI_ICON1);

      splitBtn.On(SecondaryAction, [this]() {
        ShowPopupMenuFor(splitBtn, menu);
      });
    }
  } tests;
}

void ShowButtonTests() {
  SetVisible(tests.d, true);
}