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

namespace jwt {
  //
  // **************************************************
  // Window member function definitions
  // **************************************************
  //

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
      NMHDR* hdr = (NMHDR*)l;
      wnd = (Window*)GetWindowLong(hdr->hwndFrom, GWL_USERDATA);
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

  //
  // **************************************************
  // Non-member Window interface function definitions
  // **************************************************
  //

  std::wstring ClassName(const Window& w) {
    assert(w.TheHWND() != nullptr);

    wchar_t buffer[200] = {};
    GetClassName(w.TheHWND(), buffer, sizeof(buffer)/sizeof(wchar_t));
    return buffer;
  }

  Dimension GetSize(const Window& w) {
    assert(w.TheHWND() != nullptr);

    RECT r;
    GetWindowRect(w.TheHWND(), &r);
    return Dimension(r.right - r.left, r.bottom - r.top);
  }

  Window& SetSize(Window& w, const Dimension& d) {
    assert(w.TheHWND() != nullptr);

    SetWindowPos(
      w.TheHWND(), nullptr, 0, 0,
      d.w, d.h,
      SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    return w;
  }

  Dimension GetClientSize(const Window& w) {
    assert(w.TheHWND() != nullptr);

    RECT r;
    GetClientRect(w.TheHWND(), &r);
    return Dimension(r.right - r.left, r.bottom - r.top);
  }

  Window& SetClientSize(Window& w, const Dimension& d) {
    assert(w.TheHWND() != nullptr);

    RECT r = {
      0, 0, d.w, d.h
    };

    DWORD style = Style(w);
    DWORD exStyle = ExStyle(w);

    if (style & WS_CHILD) {
      // child window --> can't have a menu
      AdjustWindowRectEx(&r, style, false, exStyle);
    }
    else {
      // non-child window --> GetMenu should be a genuine menu or null if none present
      bool hasMenu = (GetMenu(w.TheHWND()) != nullptr);
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
      w.TheHWND(), nullptr, 0, 0,
      r.right - r.left, r.bottom - r.top,
      SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

    return w;
  }

  Point GetPosition(const Window& w) {
    assert(w.TheHWND() != nullptr);

    RECT r = {};
    GetWindowRect(w.TheHWND(), &r);

    if (HasStyle(w, WS_CHILD)) {
      POINT topLeft = { r.left, r.top };
      ScreenToClient(w.TheHWND(), &topLeft);

      return Point(r.left, r.top);
    }
    else {
      return Point(r.left, r.top);
    }
  }

  Window& SetPosition(Window& w, const Point& p) {
    assert(w.TheHWND() != nullptr);

    SetWindowPos(
      w.TheHWND(), nullptr,
      p.x, p.y, 0, 0,
      SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);

    return w;
  }

  Rect GetBounds(const Window& w) {
    assert(w.TheHWND() != nullptr);

    RECT r = {};
    GetWindowRect(w.TheHWND(), &r);

    if (HasStyle(w, WS_CHILD)) {
      POINT topLeft = { r.left, r.top };
      POINT bottomRight = { r.right, r.bottom };

      ScreenToClient(w.TheHWND(), &topLeft);
      ScreenToClient(w.TheHWND(), &bottomRight);

      return Rect(Point(topLeft), Dimension(topLeft, bottomRight));
    }
    else {
      return Rect(r);
    }
  }

  Window& SetBounds(Window& w, const Rect& r) {
    assert(w.TheHWND() != nullptr);

    SetWindowPos(
      w.TheHWND(), nullptr,
      r.position.x, r.position.y, r.size.w, r.size.h,
      SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);

    return w;
  }

  std::wstring GetText(const Window& w) {
    assert(w.TheHWND() != nullptr);

    std::wstring txt(' ', GetWindowTextLength(w.TheHWND()));
    GetWindowText(w.TheHWND(), &*txt.begin(), txt.size());

    return txt;
  }

  Window& SetText(Window& w, const std::wstring& s) {
    assert(w.TheHWND() != nullptr);

    SetWindowText(w.TheHWND(), s.c_str());
    return w;
  }

  bool IsVisible(const Window& w) {
    assert(w.TheHWND() != nullptr);

    return !!(Style(w) & WS_VISIBLE);
  }

  Window& SetVisible(Window& w, bool visible) {
    assert(w.TheHWND() != nullptr);

    ShowWindow(w.TheHWND(), (visible) ? SW_SHOWNORMAL : SW_HIDE);
    return w;
  }
} // namespace jwt