#include "jwt.hpp"
#include "resource.h"

using namespace jwt;

namespace {

  struct EditTest {
    EditTest()
      : d(IDD_LISTTESTS), newString(d, IDC_NEWSTRING), left(d, IDC_LEFTLIST), right(d, IDC_RIGHTLIST)
    {
      d.On(Close, [this]() { SetVisible(d, false); });

      auto addString = [this]() {
        std::wstring s(GetText(newString));

        if (s.size() > 0) {
          AddString(this->left, s);
        }
      };

      d.On(Command, IDC_ADDSTRING, addString);
      d.On(Command, IDOK, [this, addString]() {
        if (GetFocus() == newString.TheHWND()) {
          addString();
        }
      });

      d.On(Command, IDC_LEFTTORIGHT, [this]() {
        int i = SelectedIndex(left);

        if (i >= 0) {
          AddString(right, GetString(left, i));
          DeleteString(left, i);
        }
      });

      d.On(Command, IDC_RIGHTTOLEFT, [this]() {
        auto indices = SelectedIndices(this->right);

        std::for_each(begin(indices), end(indices), [this](int i) {
          AddString(left, GetString(right, i));
        });

        DeleteStrings(right, indices);
      });

      d.On(Command, IDC_REMOVELEFTSTRING, [this]() {
        int i = SelectedIndex(left);
        if (i >= 0) {
          DeleteString(left, i);
        }
      });

      d.On(Command, IDC_REMOVERIGHTSTRING, [this]() {
        DeleteStrings(right, SelectedIndices(right));
      });
    }

    void Show() {
      SetVisible(d, true);
    }

  private:
    Dialog d;
    Edit newString;
    ListBox left;
    ListBox right;
  } listTest;

}

void ShowListTests() {
  listTest.Show();
}