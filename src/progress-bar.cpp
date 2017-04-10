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
#include "progress-bar.hpp"
#include <assert.h>

namespace jwt {

  ProgressBar::ProgressBar(Window& parent) {
    Create(parent);
  }

  ProgressBar::ProgressBar(Dialog& parent, int ctrlId) {
    hWnd_ = parent.Item(ctrlId);

    assert(hWnd_ != nullptr);
    assert(ClassName(*this) == PROGRESS_CLASS);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  ProgressBar::ProgressBar(const defer_create_t&) {
  }

  void ProgressBar::Create(Window& parent) {
    hWnd_ = CreateWindow(
      PROGRESS_CLASS, nullptr, WS_VISIBLE | WS_CHILD,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  LRESULT ProgressBar::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      break;
    }

    return 0;
  }

  //
  // Non-member ProgressBar functions
  //

  ProgressBar& SetMarquee(ProgressBar& b, bool on) {
    assert(b.TheHWND());

    SendMessage(b.TheHWND(), PBM_SETMARQUEE, on, 0);
    return b;
  }

  ProgressBar& SetValue(ProgressBar& b, int v) {
    assert(b.TheHWND());

    SendMessage(b.TheHWND(), PBM_SETPOS, v, 0);
    return b;
  }

  ProgressBar& SetRange(ProgressBar& b, int min, int max) {
    assert(b.TheHWND());
    
    SendMessage(b.TheHWND(), PBM_SETRANGE32, min, max);
    return b;
  }

} // namespace jwt