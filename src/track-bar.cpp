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
#include "track-bar.hpp"
#include <assert.h>

namespace jwt {

  TrackBar::TrackBar(Window& parent) {
    Create(parent);
  }

  TrackBar::TrackBar(Dialog& parent, int ctrlId) {
    hWnd_ = parent.Item(ctrlId);
    
    assert(hWnd_ != nullptr);
    assert(ClassName(*this) == TRACKBAR_CLASS);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  TrackBar::TrackBar(const defer_create_t&) {
  }

  void TrackBar::Create(Window& parent) {
    hWnd_ = CreateWindow(
      TRACKBAR_CLASS, nullptr, WS_VISIBLE | WS_CHILD,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  LRESULT TrackBar::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_HSCROLL:
    case WM_VSCROLL:
      onChange_();
      break;
    }

    return 0;
  }

  //
  // Non-member trackbar functions
  //
  int GetValue(const TrackBar& b) {
    assert(b.TheHWND());

    return SendMessage(b.TheHWND(), TBM_GETPOS, 0, 0);
  }

  TrackBar& SetValue(TrackBar& b, int value) {
    assert(b.TheHWND());

    SendMessage(b.TheHWND(), TBM_SETPOS, true, value);
    return b;
  }

  TrackBar& SetRange(TrackBar& b, int min, int max) {
    assert(b.TheHWND());

    SendMessage(b.TheHWND(), TBM_SETRANGEMIN, false, min);
    SendMessage(b.TheHWND(), TBM_SETRANGEMAX, true, max);
    return b;
  }


} // namespace jwt