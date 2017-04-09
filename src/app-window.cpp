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
#include "app-window.hpp"

#include <assert.h>
#include <iostream>

namespace jwt {

  using std::cout;

  const wchar_t* AppWindow::CLASS_NAME = L"AppWindow::CLASS_NAME";

  void AppWindow::Register() {
    CustomWindow<AppWindow>::Register(CLASS_NAME);
  }

  AppWindow::AppWindow()
  {
    Create();
  }

  AppWindow::AppWindow(const defer_create_t&)
  {
  }

  void AppWindow::Create() {
    Register();
    CreateWindow(
      CLASS_NAME, L"",
      WS_POPUP | WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      nullptr, nullptr, GetModuleHandle(nullptr), (void*) this
    );
    DefaultPump().RaiseReportedException();
    assert(hWnd_ != nullptr);
  }

  LRESULT AppWindow::WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_CLOSE:
      onClose_();
      break;

    case WM_NOTIFY:
    case WM_HSCROLL:
    case WM_VSCROLL:
      ReflectMessage(h, m, w, l);
      break;

    case WM_COMMAND: {
      CommandEvent::Type t = (HIWORD(w) == 0)
        ? CommandEvent::MENU
        : (HIWORD(w) == 1)
        ? CommandEvent::ACCELERATOR
        : CommandEvent::CONTROL;

      onCommand_(CommandEvent(t, LOWORD(w), l));

      if (t == CommandEvent::CONTROL) {
        ReflectMessage(h, m, w, l);
      }
    }
                     break;

    case WM_SIZE:
      if (layoutPolicy_) {
        layoutPolicy_();
      }
      break;

    case WM_SIZING:
      if (sizePolicy_) {
        RECT* r = (RECT*)l;
        Rect r2(*r);

        sizePolicy_((unsigned int) w, r2);

        *r = (RECT)r2;
      }
      break;
    }

    return CustomWindow<AppWindow>::WndProc(h, m, w, l);
  }

  AppWindow& AppWindow::Menu(UINT resource) {
    HMENU m = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(resource));
    assert(m);

    SetMenu(hWnd_, m);
    return *this;
  }

} // namespace jwt