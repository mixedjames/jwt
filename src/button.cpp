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
#include "button.hpp"
#include <assert.h>

namespace jwt {

  Button::Button(Window& parent) {
    Create(parent);
  }

  Button::Button(Dialog& parent, int buttonId) {
    hWnd_ = parent.Item(buttonId);
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  Button::Button(const defer_create_t&) {
  }

  void Button::Create(Window& parent) {
    hWnd_ = CreateWindow(
      L"BUTTON", L"", WS_VISIBLE | WS_CHILD,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  LRESULT Button::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      if (HIWORD(w) == BN_CLICKED) {
        onClick_();
        return 0;
      }
      break;
    }

    return 0;
  }

} // namespace jwt