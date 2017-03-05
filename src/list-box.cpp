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
#include "list-box.hpp"
#include <assert.h>

namespace jwt {

  ListBox::ListBox(Window& parent) {
    Create(parent);
  }

  ListBox::ListBox(Dialog& parent, int listId) {
    hWnd_ = parent.Item(listId);
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  ListBox::ListBox(const defer_create_t&) {
  }

  ListBox& ListBox::Add(const std::wstring& s) {
    assert(hWnd_ != nullptr);

    SendMessage(hWnd_, LB_ADDSTRING, 0, (LPARAM)s.c_str());
    return *this;
  }

  ListBox& ListBox::Insert(int index, const std::wstring& s) {
    assert(hWnd_ != nullptr);

    SendMessage(hWnd_, LB_INSERTSTRING, index, (LPARAM)s.c_str());
    return *this;
  }

  ListBox& ListBox::Delete(int index) {
    assert(hWnd_ != nullptr);

    SendMessage(hWnd_, LB_DELETESTRING, index, 0);
    return *this;
  }

  int ListBox::Count() const {
    assert(hWnd_ != nullptr);
    return SendMessage(hWnd_, LB_GETCOUNT, 0, 0);
  }

  int ListBox::SelectedIndex() const {
    assert(hWnd_ != nullptr);
    return SendMessage(hWnd_, LB_GETCURSEL, 0, 0);
  }

  std::vector<int> ListBox::SelectedIndices() const {
    assert(hWnd_ != nullptr);

    int l = SendMessage(hWnd_, LB_GETSELCOUNT, 0, 0);
    if (l == LB_ERR) {
      throw "LB_GETSELCOUNT failed.";
    }

    std::vector<int> v(l, -1);
    SendMessage(hWnd_, LB_GETSELITEMS, l, (LPARAM) &*v.begin());
    return v;
  }


  void ListBox::Create(Window& parent) {
    hWnd_ = CreateWindow(
      L"LISTBOX", L"", WS_VISIBLE | WS_CHILD,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      (HWND)parent, nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  LRESULT ListBox::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      break;
    }

    return 0;
  }

} // namespace jwt