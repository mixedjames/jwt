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
#include "status-bar.hpp"
#include <assert.h>

StatusBar::StatusBar(Window& parent) {
  Create(parent);
}

StatusBar::StatusBar(Dialog& parent, int statusbarId) {
  hWnd_ = parent.Item(statusbarId);
  assert(hWnd_ != nullptr);

  SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
}

StatusBar::StatusBar(const defer_create_t&) {
}

void StatusBar::Create(Window& parent) {
  hWnd_ = CreateWindow(
    STATUSCLASSNAME, L"Test", WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
    0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
    (HWND)parent, nullptr, nullptr, nullptr
  );
  assert(hWnd_ != nullptr);

  SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
}

LRESULT StatusBar::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
  switch (m) {
  case WM_COMMAND:
    break;
  }

  return 0;
}