#include "jwt.hpp"
#include "resource.h"

using namespace jwt;

namespace {

  struct StatusBarTest {
    StatusBarTest()
      : sb_(w_, { 150, 300, -1 })
    {
      SetClientSize(w_, Dimension(600, 400));
      w_.On(Close, [this]() { SetVisible(w_, false); });

      //
      // Ensure the statusbar gets send WM_SIZE messages so it can autosize
      //

      w_.LayoutPolicy([this]() {
        SetSize(sb_, GetClientSize(w_));
      });
      SetSize(sb_, GetClientSize(w_));

      //
      // Set the text in the various parts
      //

      SetText(sb_, 0, L"Part 0");
      SetText(sb_, 1, L"Part 1");
      SetText(sb_, 2, L"Part 2");
    }

    void Show() {
      SetVisible(w_, true);
    }

  private:
    AppWindow w_;
    StatusBar sb_;

  } statusBarTest;

}

void ShowStatusBarTests() {
  statusBarTest.Show();
}