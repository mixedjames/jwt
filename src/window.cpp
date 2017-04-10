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
#include "message-pump.hpp"
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
        wnd = (Window*)GetWindowLongPtr((HWND)l, GWLP_USERDATA);
      }
      break;

    case WM_NOTIFY: {
      NMHDR* hdr = (NMHDR*)l;
      wnd = (Window*)GetWindowLongPtr(hdr->hwndFrom, GWLP_USERDATA);
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

  LRESULT SafeSendMessage(Window& wnd, UINT m, WPARAM w, LPARAM l) {
    LRESULT lr = SendMessage(wnd.TheHWND(), m, w, l);
    DefaultPump().RaiseReportedException();
    return lr;
  }

  LRESULT SafeSendMessage(const Window& wnd, UINT m, WPARAM w, LPARAM l) {
    LRESULT lr = SendMessage(wnd.TheHWND(), m, w, l);
    DefaultPump().RaiseReportedException();
    return lr;
  }

  std::wstring ClassName(const Window& w) {
    assert(w.TheHWND() != nullptr);

    // FIXME: currently implemented using a fixed-size buffer.
    //        would be better if we dynamically allocated this somehow.

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

    // GetWindowRect always returns screen coordinates. See GetBounds
    // impl. for the altered symantics we provide.

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

    // GetWindowRect always returns screen coordinates, however we try
    // to implement slightly more helpful (if (?)less consistent) symantics:
    //   - Top level window bounds are specified in screen coordinates
    //   - Child window bounds are specified in the coordinates of their parent
    //
    // So, if w is a child window, we have to call ScreenToClient to perform
    // the conversion.

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
    // Making GetWindowText play nicely with standard C++ strings is
    // a tad more complicated than would be ideal - problems are:
    // (a) Although wstrings are null terminated in practice, the null character
    //     is is memory that is not accessible to the application
    // (b) Some methods (i.e. standard streams) use the actual length of the string,
    //     not the strlen(...) length, and these methods get confused if an
    //     additional null is present (i.e. wcout prints a blank character)
    //
    // Strategy we adopt is therefore slightly nuanced...
    // (a) First create a string big enough for the whole window text including
    //     a terminating null & call GetWindowText using this
    // (b) If a non-empty string was returned, the string is resized to lose the
    //     final character.

    assert(w.TheHWND() != nullptr);

    std::wstring txt(GetWindowTextLength(w.TheHWND()) + 1, ' ');
    GetWindowText(w.TheHWND(), &*txt.begin(), (int) txt.size());

    if (txt.size() > 0) {
      txt.resize(txt.size() - 1);
    }

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

  Dimension CalculateExtentOfChildren(const Window& w) {
    Dimension extent;

    // Note: this const_cast if safe because ForEachChild promises not
    //       to mutate the target Window& unless the specified callback does.
    //       Since we control the callback & we don't mess with it then
    //       we are ok.
    //
    ForEachChild(const_cast<Window&>(w), [&w, &extent](HWND h) {
      RECT r;
      GetClientRect(h, &r);
      MapWindowPoints(h, w.TheHWND(), (POINT*)&r, 2);

      extent.w = (extent.w < r.right) ? r.right : extent.w;
      extent.h = (extent.h < r.bottom) ? r.bottom : extent.h;
    });

    return extent;
  }

} // namespace jwt