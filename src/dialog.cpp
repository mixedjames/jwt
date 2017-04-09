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
#include "dialog.hpp"
#include "message-pump.hpp"

namespace jwt {

  Dialog::Dialog(int resourceId) {
    Create(resourceId);
  }

  Dialog::Dialog(Window& parent, int resourceId) {
    Create(parent, resourceId);
  }

  Dialog::Dialog(const defer_create_t&) {
  }

  Dialog::~Dialog() {
    if (hWnd_ && GetWindowLongPtr(hWnd_, GWLP_USERDATA)) {
      DefaultPump().RemoveDialog(hWnd_);
      DestroyWindow(hWnd_);
    }
  }

  HWND Dialog::Item(int id) {
    return GetDlgItem(hWnd_, id);
  }

  void Dialog::Create(int resourceId) {
    CreateDialogParam(
      GetModuleHandle(nullptr),
      MAKEINTRESOURCE(resourceId),
      nullptr,
      DlgProcAdapter,
      (LPARAM) this
    );
    DefaultPump().AddDialog(hWnd_);
  }

  void Dialog::Create(Window& parent, int resourceId) {
    CreateDialogParam(
      GetModuleHandle(nullptr),
      MAKEINTRESOURCE(resourceId),
      parent.TheHWND(),
      DlgProcAdapter,
      (LPARAM) this
    );
    DefaultPump().RaiseReportedException();
    DefaultPump().AddDialog(hWnd_);
  }

  INT_PTR Dialog::DlgProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND: {
      CommandEvent::Type t = (HIWORD(w) == 0)
        ? CommandEvent::MENU
        : (HIWORD(w) == 1)
        ? CommandEvent::ACCELERATOR
        : CommandEvent::CONTROL;

      onCommand_(CommandEvent(t, LOWORD(w), l));

      if (l) {
        ReflectMessage(h, m, w, l);
      }
    }
    break;

    case WM_NOTIFY:
    case WM_HSCROLL:
    case WM_VSCROLL:
      ReflectMessage(h, m, w, l);
      return TRUE;

    case WM_CLOSE:
      onClose_();
      return TRUE;
    }

    return FALSE;
  }

  INT_PTR Dialog::PrivateDlgProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_INITDIALOG) {
      hWnd_ = h;
    }

    try {
      return DlgProc(h, m, w, l);
    }
    catch (...) {
      DefaultPump().ReportException(std::current_exception());
      return FALSE;
    }
  }

  INT_PTR CALLBACK Dialog::DlgProcAdapter(HWND h, UINT m, WPARAM w, LPARAM l) {
    Dialog* dlg;

    if (m == WM_INITDIALOG) {
      dlg = (Dialog*)l;
      SetWindowLongPtr(h, GWLP_USERDATA, (LONG_PTR)dlg);
    }
    else {
      dlg = (Dialog*)GetWindowLongPtr(h, GWLP_USERDATA);
    }

    if (dlg) {
      return dlg->PrivateDlgProc(h, m, w, l);
    }
    else {
      return FALSE;
    }
  }
} // namespace jwt