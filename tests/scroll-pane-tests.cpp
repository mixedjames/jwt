#include "jwt.hpp"
#include "resource.h"

#include <iostream>

using namespace jwt;

namespace {

  struct ScrollPaneTest {
    ScrollPaneTest()
      : scrollPane_(w_), d_(scrollPane_, IDD_SCROLLPANETESTS)
    {

      //
      // Set up the window & scrollpane...
      //

      SetText(w_, L"ScrollPane Tests");
      SetClientSize(w_, Dimension(600, 400));
      w_.Menu(IDR_SCROLLPANETESTSMENU);
      w_.LayoutPolicy([this]() {
        SetSize(scrollPane_, GetClientSize(w_));
      });

      scrollPane_.AlwaysOn(false);
      scrollPane_.Extent(CalculateExtentOfChildren(scrollPane_));

      SetClientSize(w_, GetSize(d_));

      //
      // Handle user interaction with window & controls...
      //

      w_.On(Close, [this]() {
        SetVisible(w_, false);
      });

      w_.On(Command, ID_FITWINDOWTOCONTENTS, [this]() {
        if (scrollPane_.AlwaysOn()) {
          SetClientSize(scrollPane_, GetSize(d_));
          SetClientSize(w_, GetSize(scrollPane_));
        }
        else {
          SetClientSize(w_, GetSize(d_));
        }
      });

      w_.On(Command, ID_TOGGLESCROLLBARSALWAYSON, [this]() {
        scrollPane_.AlwaysOn(!scrollPane_.AlwaysOn());
      });
      
      w_.On(Command, ID_CLOSE, [this]() {
        SetVisible(w_, false);
      });

      d_.On(Command, ID_SCROLLTOBOTTOMRIGHT, [this]() {
        Dimension sz = GetSize(d_);
        scrollPane_.Position(Point(sz.w, sz.h));
      });

      d_.On(Command, ID_SCROLLTOTOPLEFT, [this]() {
        scrollPane_.Position(Point(0,0));
      });
    }

    void Show() {
      SetVisible(w_, true);
    }

  private:
    AppWindow w_;
    ScrollPane scrollPane_;
    Dialog d_;

  } scrollPaneTest;

}

void ShowScrollPaneTests() {
  scrollPaneTest.Show();
}