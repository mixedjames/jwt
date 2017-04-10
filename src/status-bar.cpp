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

namespace jwt {

  StatusBar::StatusBar(Window& parent) {
    Create(parent);
  }

  StatusBar::StatusBar(Window& parent, const std::initializer_list<int>& parts) {
    Create(parent);
    SetParts(*this, parts);
  }

  StatusBar::StatusBar(Dialog& parent, int statusbarId) {
    hWnd_ = parent.Item(statusbarId);
    
    assert(hWnd_ != nullptr);
    assert(ClassName(*this) == STATUSCLASSNAME);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  StatusBar::StatusBar(const defer_create_t&) {
  }

  void StatusBar::Create(Window& parent) {
    hWnd_ = CreateWindow(
      STATUSCLASSNAME, L"Test", WS_VISIBLE | WS_CHILD | SBARS_SIZEGRIP,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  LRESULT StatusBar::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      break;
    }

    return 0;
  }

  //
  // Non-member statusbar functions
  //

  StatusBar& SetParts(StatusBar& s, const std::initializer_list<int>& l) {
    assert(s.TheHWND());
    assert(l.size() > 0 && l.size() <= 256);

    std::vector<int> parts(l);
    SendMessage(s.TheHWND(), SB_SETPARTS, parts.size(), (LPARAM) &*parts.begin());

    return s;
  }

  StatusBar& SetText(StatusBar& s, int part, const std::wstring& txt) {
    assert(s.TheHWND());
    assert(part >= 0 && part <= 255);

    SendMessage(s.TheHWND(), SB_SETTEXT, MAKEWORD(part, 0), (LPARAM) txt.c_str());

    return s;
  }

  std::wstring GetText(StatusBar& s, int part) {
    assert(s.TheHWND());
    assert(part >= 0 && part <= 255);

    int length = LOWORD(SendMessage(s.TheHWND(), SB_GETTEXTLENGTH, part, 0));
    if (length < 1) {
      return L"";
    }

    std::wstring txt(length, ' ');
    SendMessage(s.TheHWND(), SB_GETTEXT, part, (LPARAM) &*txt.begin());

    txt.resize(txt.size() - 1);

    return txt;
  }


} // namespace jwt