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
#include "edit.hpp"
#include <assert.h>

namespace jwt {

  //
  // **************************************************
  // Edit member function definitions
  // **************************************************
  //

  Edit::Edit(Window& parent, const std::wstring& txt, DWORD flags) {
    Create(parent, flags);
    SetText(*this, txt);
  }

  Edit::Edit(Dialog& parent, int editId) {
    hWnd_ = parent.Item(editId);
    assert(hWnd_ != nullptr);
    assert(ClassName(*this) == L"Edit");

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  Edit::Edit(const defer_create_t&) {
  }

  void Edit::Create(Window& parent, DWORD flags) {
    hWnd_ = CreateWindow(
      L"EDIT", L"", WS_VISIBLE | WS_CHILD | flags,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  LRESULT Edit::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      break;
    }

    return 0;
  }

  //
  // **************************************************
  // Non-member Button function definitions
  // **************************************************
  //

  std::wstring GetSelectedText(const Edit& e) {
    auto s = GetText(e);
    DWORD start = 0;
    DWORD end = 0;

    SendMessage(e.TheHWND(), EM_GETSEL, (WPARAM) &start, (LPARAM) &end);

    return s.substr(start, end - start);
  }

  Edit& ReplaceSelectedText(Edit& e, const std::wstring& txt) {
    SendMessage(e.TheHWND(), EM_REPLACESEL, TRUE, (LPARAM) txt.c_str());
    return e;
  }

  std::wstring GetCueBanner(const Edit& e) {
    wchar_t buffer[200] = {};
    SendMessage(e.TheHWND(), EM_GETCUEBANNER, (WPARAM) buffer, sizeof(buffer) / sizeof(wchar_t));
    return buffer;
  }

  Edit& SetCueBanner(Edit& e, const std::wstring& txt) {
    SendMessage(e.TheHWND(), EM_SETCUEBANNER, false, (LPARAM) txt.c_str());
    return e;
  }


} // namespace jwt