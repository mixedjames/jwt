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
#pragma once

namespace jwt {

  template<typename UniqueTag>
  ATOM CustomWindow<UniqueTag>::atom_ = 0;

  template<typename UniqueTag>
  CustomWindow<UniqueTag>::~CustomWindow() {
    if (GetWindowLongPtr(hWnd_, GWL_USERDATA)) {
      SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) nullptr);
      DestroyWindow(hWnd_);
    }
  }

  template<typename UniqueTag>
  void CustomWindow<UniqueTag>::Register(const wchar_t* clsName) {
    if (!atom_) {
      WNDCLASS wc = {};
      wc.lpszClassName = clsName;
      wc.lpfnWndProc = WndProcAdapter;
      wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
      wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

      atom_ = RegisterClass(&wc);
      assert(atom_ != 0);
    }
  }

  template<typename UniqueTag>
  LRESULT CustomWindow<UniqueTag>::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CLOSE:
      return 0;

    default:
      return DefWindowProc(h, m, w, l);
    }
  }

  template<typename UniqueTag>
  LRESULT CALLBACK CustomWindow<UniqueTag>::WndProcAdapter(HWND h, UINT m, WPARAM w, LPARAM l) {
    CustomWindow<UniqueTag>* wnd;

    if (m == WM_NCCREATE) {
      CREATESTRUCT* cs = (CREATESTRUCT*)l;
      wnd = (CustomWindow<UniqueTag>*)cs->lpCreateParams;

      if (!wnd) {
        wnd = new UniqueTag(defer_create);
      }

      SetWindowLongPtr(h, GWL_USERDATA, (LONG_PTR)wnd);
    }
    else if (m == WM_DESTROY) {
      wnd = (CustomWindow<UniqueTag>*)GetWindowLongPtr(h, GWL_USERDATA);
      if (wnd) {
        // app != nullptr means that DestroyWindow was called externally
        // Set GWL_USERDATA to nullptr to make the destructor aware that it doesn't need to
        // destroy the window itself
        SetWindowLongPtr(h, GWL_USERDATA, (LONG_PTR) nullptr);
        delete wnd;
      }
      else {
        // app == nullptr means that this message was triggered from the destructor
        // Nothing to do in this case
      }
      return DefWindowProc(h, m, w, l);
    }
    else {
      wnd = (CustomWindow<UniqueTag>*)GetWindowLongPtr(h, GWL_USERDATA);
    }

    if (wnd) {
      return wnd->PrivateWndProc(h, m, w, l);
    }
    else {
      return DefWindowProc(h, m, w, l);
    }

  }


  template<typename UniqueTag>
  LRESULT CustomWindow<UniqueTag>::PrivateWndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_NCCREATE) {
      hWnd_ = h;
    }

    return WndProc(h, m, w, l);
  }

}