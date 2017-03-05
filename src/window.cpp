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
#include "window.hpp"
#include <assert.h>

Dimension Window::Size() const {
  assert(hWnd_ != nullptr);

  RECT r;
  GetWindowRect(hWnd_, &r);
  return Dimension(r.right - r.left, r.bottom - r.top);
}

Window& Window::Size(const Dimension& d) {
  assert(hWnd_ != nullptr);

  SetWindowPos(
    hWnd_, nullptr, 0, 0,
    d.w, d.h,
    SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
  return *this;
}

Dimension Window::ClientSize() const {
  assert(hWnd_ != nullptr);

  RECT r;
  GetClientRect(hWnd_, &r);
  return Dimension(r.right - r.left, r.bottom - r.top);
}

Window& Window::ClientSize(const Dimension& d) {
  assert(hWnd_ != nullptr);

  RECT r = {
    0, 0, d.w, d.h
  };

  DWORD style = Style();
  DWORD exStyle = ExStyle();

  if (style & WS_CHILD) {
    // child window --> can't have a menu
    AdjustWindowRectEx(&r, style, false, exStyle);
  }
  else {
    // non-child window --> GetMenu should be a genuine menu or null if none present
    bool hasMenu = (GetMenu(hWnd_) != nullptr);
    AdjustWindowRectEx(&r, style, hasMenu, exStyle);
  }

  // AdjustWindowRectEx doesn't account for scrollbars
  if (style & WS_HSCROLL) {
    r.right += GetSystemMetrics(SM_CXVSCROLL);
  }

  if (style & WS_VSCROLL) {
    r.bottom += GetSystemMetrics(SM_CYHSCROLL);
  }

  SetWindowPos(
    hWnd_, nullptr, 0, 0,
    r.right - r.left, r.bottom - r.top,
    SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

  return *this;
}

std::wstring Window::Text() const {
  assert(hWnd_ != nullptr);

  std::wstring txt(' ', GetWindowTextLength(hWnd_));
  GetWindowText(hWnd_, &*txt.begin(), txt.size());

  return txt;
}

Window& Window::Text(const std::wstring& s) {
  assert(hWnd_ != nullptr);

  SetWindowText(hWnd_, s.c_str());
  return *this;
}

bool Window::Visible() const {
  assert(hWnd_ != nullptr);

  return !!(Style() & WS_VISIBLE);
}

Window& Window::Visible(bool visible) {
  assert(hWnd_ != nullptr);

  ShowWindow(hWnd_, (visible) ? SW_SHOWNORMAL : SW_HIDE);
  return *this;
}

LRESULT Window::ReflectMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
  Window* wnd = nullptr;

  switch (m) {
  case WM_COMMAND:
  case WM_VSCROLL:
  case WM_HSCROLL:
    if (l) {
      wnd = (Window*)GetWindowLongPtr((HWND)l, GWL_USERDATA);
    }
    break;

  case WM_NOTIFY: {
      NMHDR* hdr = (NMHDR*) l;
      wnd = (Window*) GetWindowLong(hdr->hwndFrom, GWL_USERDATA);
    }
    break;

  default:
    assert(false);
  }

  if (wnd) {
    return wnd->HandleReflectedMessage(h, m, w, l);
  }
  else {
    return 0;
  }
}

LRESULT Window::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
  assert(hWnd_ != nullptr);

  return DefWindowProc(h, m, w, l);
}