/*
  James' Windows Toolkit (JWT)
  Copyright (C) 2017 James Heggie

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "libraries.hpp"
#include "scroll-pane.hpp"

#include <iostream>

namespace jwt {

  const wchar_t* ScrollPane::CLASS_NAME = L"ScrollPane::CLASS_NAME";

  void ScrollPane::Register() {
    CustomWindow<ScrollPane>::Register(CLASS_NAME);
  }

  ScrollPane::ScrollPane(Window& parent)
    : flags_(0), lineIncrement_(1, 1), pageIncrement_(-1, -1), scrollPolicy_(DefaultScrollPolicy)
  {
    Create(parent);
  }

  ScrollPane::ScrollPane(const defer_create_t&)
    : flags_(0), lineIncrement_(1, 1), pageIncrement_(-1, -1), scrollPolicy_(DefaultScrollPolicy)
  {
  }

  void ScrollPane::Create(Window& parent) {
    Register();
    CreateWindow(CLASS_NAME, L"",
      WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL,
      0, 0, 0, 0,
      parent.TheHWND(), nullptr, GetModuleHandle(nullptr), (LPVOID) this
    );
    DefaultPump().RaiseReportedException();

    assert(hWnd_);
  }

  LRESULT ScrollPane::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_SIZE:
      ConfigScrollbars();
      break;

    case WM_HSCROLL:
      HandleHScroll(LOWORD(w));
      break;

    case WM_VSCROLL:
      HandleVScroll(LOWORD(w));
      break;
    }

    return CustomWindow<ScrollPane>::WndProc(h, m, w, l);
  }

  ScrollPane& ScrollPane::Position(const Point& position) {
    Dimension vSize = GetClientSize(*this);
    Point dP = position_;

    position_ = position;

    // Note: extra parentheses are required - windows headers define max/min macros
    // which munch the std:: C++ ones; the extra brackets prevent this. I don't want
    // to compile with wierd macros if I can avoid it so this seemed better than using
    // #define NO_MINMAX
    //

    position_.x = (std::min)(position_.x, extent_.w - vSize.w);
    position_.x = (std::max)(position_.x, 0);

    position_.y = (std::min)(position_.y, extent_.h - vSize.h);
    position_.y = (std::max)(position_.y, 0);

    dP.x = position_.x - dP.x;
    dP.y = position_.y - dP.y;

    CommonConfigScrollbars();

    scrollPolicy_(*this, position_, dP);
    return *this;
  }

  ScrollPane& ScrollPane::Extent(const Dimension& extent) {
    extent_ = extent;
    ConfigScrollbars();
    return *this;
  }

  ScrollPane& ScrollPane::AlwaysOn(bool b) {
    if (b) {
      flags_ |= ALWAYS_ON;
    }
    else {
      flags_ &= ~ALWAYS_ON;
    }
    ConfigScrollbars();
    return *this;
  }

  ScrollPane& ScrollPane::LineIncrement(const Dimension& newIncrement) {
    lineIncrement_ = newIncrement;
    return *this;
  }

  ScrollPane& ScrollPane::PageIncrement(const Dimension& newIncrement) {
    pageIncrement_ = newIncrement;
    return *this;
  }

  void ScrollPane::ConfigScrollbars() {
    if (AlwaysOn()) {
      ConfigAlwaysOnScrollbars();
    }
    else {
      ConfigOptionalScrollbars();
    }
  }

  // Basic pattern common to both always-on & as-needed bars:
  // (1) Decide if you need the scrollbars (always yes in this case)
  // (2) Call CommonConfigScrollbars() to ensure scrollbars represent
  //     the current extent and make sure the current scroll region is
  //     valid - i.e. no scroll when content smaller than viewport etc.
  //
  void ScrollPane::ConfigAlwaysOnScrollbars() {
    ShowScrollBar(hWnd_, SB_HORZ, true);
    ShowScrollBar(hWnd_, SB_VERT, true);

    CommonConfigScrollbars();
  }

  // Basic pattern common to both always-on & as-needed bars:
  // (1) Decide if you need the scrollbars
  // (2) Call CommonConfigScrollbars() to ensure scrollbars represent
  //     the current extent and make sure the current scroll region is
  //     valid - i.e. no scroll when content smaller than viewport etc.
  //
  void ScrollPane::ConfigOptionalScrollbars() {
    Dimension outerSize = GetSize(*this);
    Dimension innerSize(
      outerSize.w - GetSystemMetrics(SM_CXVSCROLL),
      outerSize.h - GetSystemMetrics(SM_CXHSCROLL)
    );
    SCROLLINFO si = {};

    if (extent_.w <= outerSize.w && extent_.h <= outerSize.h) {
      // Both content dimensions <= outer size so no scrollbars needed
      ShowScrollBar(hWnd_, SB_HORZ, false);
      ShowScrollBar(hWnd_, SB_VERT, false);
    }
    else if (extent_.w > outerSize.w && extent_.h <= innerSize.h) {
      // Content is too wide but height is < inner height so can add
      // horizontal scrollbar without needing vertical one.
      ShowScrollBar(hWnd_, SB_HORZ, true);
      ShowScrollBar(hWnd_, SB_VERT, false);
    }
    else if (extent_.h > outerSize.h && extent_.w <= innerSize.w) {
      // Content is too tall but width is < inner width so can add
      // vertical scrollbar without needing horizontal one.
      ShowScrollBar(hWnd_, SB_HORZ, false);
      ShowScrollBar(hWnd_, SB_VERT, true);
    }
    else {
      // All other cases need both scrollbars
      ShowScrollBar(hWnd_, SB_HORZ, true);
      ShowScrollBar(hWnd_, SB_VERT, true);
    }
    UpdateWindow(hWnd_);

    CommonConfigScrollbars();
  }

  void ScrollPane::CommonConfigScrollbars() {
    SCROLLINFO si = {};
    Dimension innerSize = GetClientSize(*this);
    
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;

    si.nMax = extent_.w;
    si.nPage = innerSize.w;
    SetScrollInfo(hWnd_, SB_HORZ, &si, TRUE);

    si.nMax = extent_.h;
    si.nPage = innerSize.h;
    SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);

    if (extent_.w <= innerSize.w) {
      // Content width is smaller than or equal to the viewport width...
      // Scroll to x=0

      Point deltaP(-position_.x, 0);
      position_.x = 0;
      scrollPolicy_(*this, position_, deltaP);
    }
    else if (extent_.w - position_.x < innerSize.w) {
      // Width of content is greater than viewport width BUT scrolling
      // has put the rightmost edge within the viewport.
      //
      // In this case, we should adjust the scroll position so that
      // the right edge matches the edge of the viewport

      Point deltaP(extent_.w - innerSize.w - position_.x, 0);
      position_.x = extent_.w - innerSize.w;

      scrollPolicy_(*this, position_, deltaP);
    }

    if (extent_.h <= innerSize.h) {
      Point deltaP(0, -position_.y);
      position_.y = 0;

      scrollPolicy_(*this, position_, deltaP);
    }
    else if (extent_.h - position_.y < innerSize.h) {
      Point deltaP(0, extent_.h - innerSize.h - position_.y);
      position_.y = extent_.h - innerSize.h;

      scrollPolicy_(*this, position_, deltaP);
    }

  }

  void ScrollPane::HandleHScroll(int action) {
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(hWnd_, SB_HORZ, &si);

    switch (action) {
    case SB_LINELEFT:
      si.nPos -= lineIncrement_.w;
      break;

    case SB_LINERIGHT:
      si.nPos += lineIncrement_.w;
      break;

    case SB_PAGELEFT:
      si.nPos -= (pageIncrement_.w != -1) ? pageIncrement_.w : si.nPage;
      break;

    case SB_PAGERIGHT:
      si.nPos += (pageIncrement_.w != -1) ? pageIncrement_.w : si.nPage;
      break;

    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hWnd_, SB_HORZ, &si, TRUE);
    GetScrollInfo(hWnd_, SB_HORZ, &si);

    if (si.nPos != position_.x) {
      Point deltaP(si.nPos - position_.x, 0);
      position_.x = si.nPos;

      scrollPolicy_(*this, position_, deltaP);
      UpdateWindow(hWnd_);
    }
  }

  void ScrollPane::HandleVScroll(int action) {
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(hWnd_, SB_VERT, &si);

    switch (action) {
    case SB_LINEUP:
      si.nPos -= lineIncrement_.h;
      break;

    case SB_LINEDOWN:
      si.nPos += lineIncrement_.h;
      break;

    case SB_PAGEUP:
      si.nPos -= (pageIncrement_.h == -1) ? pageIncrement_.h : si.nPage;
      break;

    case SB_PAGEDOWN:
      si.nPos += (pageIncrement_.h == -1) ? pageIncrement_.h : si.nPage;
      break;

    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);
    GetScrollInfo(hWnd_, SB_VERT, &si);

    if (si.nPos != position_.y) {
      Point deltaP(0, si.nPos - position_.y);
      position_.y = si.nPos;

      scrollPolicy_(*this, position_, deltaP);
      UpdateWindow(hWnd_);
    }
  }

  //
  // Non-member ScrollPane functions
  //
  void DefaultScrollPolicy(ScrollPane& pane, const Point& p, const Point& dp) {
    if (dp.x || dp.y) {
      ScrollWindow(pane.TheHWND(), -dp.x, -dp.y, nullptr, nullptr);
    }
  }

} // namespace jwt