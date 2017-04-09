#include "jwt.hpp"
#include "resource.h"

using namespace jwt;

namespace {

  struct EditTest {
    Dialog d;
    Edit singleLine1;
    Edit singleLine2;

    EditTest()
      : d(IDD_EDITTESTS), singleLine1(d, IDC_SINGLELINE1), singleLine2(d, IDC_SINGLELINE2)
    {
      d.On(Close, [this]() { SetVisible(d, false); });

      d.On(Command, IDC_SHOWSELECTEDTEXT, [this]() {
        std::wstring txt(GetSelectedText(singleLine1));

        if (txt.size() > 0) {
          MessageBox(d.TheHWND(), txt.c_str(), nullptr, MB_ICONINFORMATION);
        }
        else {
          MessageBox(d.TheHWND(), L"No text was selected!", nullptr, MB_ICONERROR);
        }
      });
      d.On(Command, IDC_SWITCHSELECTIONS, [this]() {
        auto tmp = GetSelectedText(singleLine1);
        ReplaceSelectedText(singleLine1, GetSelectedText(singleLine2));
        ReplaceSelectedText(singleLine2, tmp);
      });

      SetCueBanner(singleLine1, L"Type & select some text");
      SetCueBanner(singleLine2, L"And some more");
    }
  } editTest;

}

void ShowEditTests() {
  SetVisible(editTest.d, true);
}