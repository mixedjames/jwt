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
      Dialog& d = this->d;
      Edit& singleLine1 = this->singleLine1;
      Edit& singleLine2 = this->singleLine2;

      d.On(Close, [&d]() { SetVisible(d, false); });
      d.On(Command, IDC_SHOWSELECTEDTEXT, [&d, &singleLine1]() {
        MessageBox(d.TheHWND(), GetSelectedText(singleLine1).c_str(), nullptr, MB_ICONINFORMATION);
      });
      d.On(Command, IDC_SWITCHSELECTIONS, [&singleLine1, &singleLine2]() {
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