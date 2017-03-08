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
    : flags_(0)
  {
    Create(parent);
  }

  ScrollPane::ScrollPane(const defer_create_t&)
    : flags_(0)
  {
  }

  void ScrollPane::Create(Window& parent) {
    Register();
    CreateWindow(CLASS_NAME, L"",
      WS_VISIBLE | WS_CHILD | WS_HSCROLL | WS_VSCROLL,
      0, 0, 0, 0,
      parent.TheHWND(), nullptr, GetModuleHandle(nullptr), (LPVOID) this
    );
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

  ScrollPane& ScrollPane::CalculateScrollableExtent() {
    ForEachChild(*this, [this](HWND h) {
      RECT r;
      GetClientRect(h, &r);
      MapWindowPoints(h, hWnd_, (POINT*)&r, 2);

      extent_.w = (extent_.w < r.right) ? r.right : extent_.w;
      extent_.h = (extent_.h < r.bottom) ? r.bottom : extent_.h;
    });

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

  void ScrollPane::ConfigScrollbars() {
    if (AlwaysOn()) {
      ConfigAlwaysOnScrollbars();
    }
    else {
      ConfigOptionalScrollbars();
    }
  }

  void ScrollPane::ConfigAlwaysOnScrollbars() {
    ShowScrollBar(hWnd_, SB_HORZ, true);
    ShowScrollBar(hWnd_, SB_VERT, true);

    Dimension size = GetClientSize(*this);
    SCROLLINFO si = {};

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;

    si.nMin = 0;
    si.nMax = extent_.w;
    si.nPage = size.w;
    SetScrollInfo(hWnd_, SB_HORZ, &si, TRUE);

    if (extent_.w <= size.w) {
      ScrollWindow(hWnd_, -position_.x, 0, nullptr, nullptr);
      position_.x = 0;
    }
    else if (extent_.w + position_.x < size.w) {
      int dx = size.w - extent_.w - position_.x;
      position_.x = size.w - extent_.w;
      ScrollWindow(hWnd_, dx, 0, nullptr, nullptr);
    }

    si.nMin = 0;
    si.nMax = extent_.h;
    si.nPage = size.h;
    SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);

    if (extent_.h <= size.h) {
      ScrollWindow(hWnd_, -position_.y, 0, nullptr, nullptr);
      position_.y = 0;
    }
    else if (extent_.h + position_.y < size.h) {
      int dy = size.h - extent_.h - position_.y;
      position_.y = size.h - extent_.h;

      ScrollWindow(hWnd_, 0, dy, nullptr, nullptr);
    }

    UpdateWindow(hWnd_);
  }

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

    innerSize = GetClientSize(*this);

    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE;
    si.nMin = 0;

    si.nMax = extent_.w;
    si.nPage = innerSize.w;
    SetScrollInfo(hWnd_, SB_HORZ, &si, TRUE);

    si.nMax = extent_.h;
    si.nPage = innerSize.h;
    SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);

    if (position_.x + extent_.w < innerSize.w) {
      if (position_.x < 0) {
        int dx = innerSize.w - extent_.w - position_.x;

        position_.x += dx;
        ScrollWindow(hWnd_, dx, 0, nullptr, nullptr);
      }
      else {
        ScrollWindow(hWnd_, -position_.x, 0, nullptr, nullptr);
        position_.x = 0;
      }
    }

    if (position_.y + extent_.h < innerSize.h) {
      if (position_.y < 0) {
        int dy = innerSize.h - extent_.h - position_.y;

        position_.y += dy;
        ScrollWindow(hWnd_, 0, dy, nullptr, nullptr);
      }
      else {
        ScrollWindow(hWnd_, 0, -position_.y, nullptr, nullptr);
        position_.y = 0;
      }
    }
  }

  void ScrollPane::HandleHScroll(int action) {
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(hWnd_, SB_HORZ, &si);

    switch (action) {
    case SB_LINELEFT:
      si.nPos -= 1;
      break;

    case SB_LINERIGHT:
      si.nPos += 1;
      break;

    case SB_PAGELEFT:
      si.nPos -= si.nPage;
      break;

    case SB_PAGERIGHT:
      si.nPos += si.nPage;
      break;

    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hWnd_, SB_HORZ, &si, TRUE);
    GetScrollInfo(hWnd_, SB_HORZ, &si);

    if (si.nPos != position_.x) {
      ScrollWindow(hWnd_, -position_.x - si.nPos, 0, nullptr, nullptr);
      UpdateWindow(hWnd_);

      position_.x = -si.nPos;
    }
  }

  void ScrollPane::HandleVScroll(int action) {
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_ALL;

    GetScrollInfo(hWnd_, SB_VERT, &si);

    switch (action) {
    case SB_LINEUP:
      si.nPos -= 1;
      break;

    case SB_LINEDOWN:
      si.nPos += 1;
      break;

    case SB_PAGEUP:
      si.nPos -= si.nPage;
      break;

    case SB_PAGEDOWN:
      si.nPos += si.nPage;
      break;

    case SB_THUMBTRACK:
      si.nPos = si.nTrackPos;
      break;
    }

    si.fMask = SIF_POS;
    SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);
    GetScrollInfo(hWnd_, SB_VERT, &si);

    if (si.nPos != position_.y) {
      ScrollWindow(hWnd_, 0, -position_.y - si.nPos, nullptr, nullptr);
      UpdateWindow(hWnd_);

      position_.y = -si.nPos;
    }
  }

} // namespace jwt