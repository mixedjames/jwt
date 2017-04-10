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

  //
  // **************************************************
  // ListBox member function definitions
  // **************************************************
  //

  ListBox::ListBox(Window& parent) {
    Create(parent);
  }

  ListBox::ListBox(Dialog& parent, int listId) {
    hWnd_ = parent.Item(listId);

    assert(hWnd_ != nullptr);
    assert(ClassName(*this) == L"ListBox");


    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  ListBox::ListBox(const defer_create_t&) {
  }

  void ListBox::Create(Window& parent) {
    hWnd_ = CreateWindow(
      L"ListBox", L"", WS_VISIBLE | WS_CHILD,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWLP_USERDATA, (LONG_PTR) this);
  }

  LRESULT ListBox::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      break;
    }

    return 0;
  }

  //
  // **************************************************
  // Non-member ListBox function definitions
  // **************************************************
  //

  ListBox& AddString(ListBox& l, const std::wstring& s) {
    assert(l.TheHWND() != nullptr);

    SendMessage(l.TheHWND(), LB_ADDSTRING, 0, (LPARAM)s.c_str());
    return l;
  }

  ListBox& InsertString(ListBox& l, int index, const std::wstring& s) {
    assert(l.TheHWND() != nullptr);

    SendMessage(l.TheHWND(), LB_INSERTSTRING, index, (LPARAM)s.c_str());
    return l;
  }

  ListBox& DeleteString(ListBox& l, int index) {
    assert(l.TheHWND() != nullptr);

    SendMessage(l.TheHWND(), LB_DELETESTRING, index, 0);
    return l;
  }

  std::wstring GetString(ListBox& l, int index) {
    assert(l.TheHWND());
    
    int length = SendMessage(l.TheHWND(), LB_GETTEXTLEN, index, 0);
    if (length == LB_ERR) {
      // FIXME: should throw something more useful
      throw "LB_GETTEXTLEN failed.";
    }

    std::wstring s(length + 1, ' ');
    SendMessage(l.TheHWND(), LB_GETTEXT, index, (LPARAM) &*s.begin());

    if (s.size() > 0) {
      s.resize(s.size() - 1);
    }

    return s;
  }

  int Count(ListBox& l) {
    assert(l.TheHWND() != nullptr);
    return (int)SendMessage(l.TheHWND(), LB_GETCOUNT, 0, 0);
  }

  int SelectedIndex(const ListBox& l) {
    assert(l.TheHWND() != nullptr);
    assert(!HasStyle(l, LBS_MULTIPLESEL));

    return (int) SendMessage(l.TheHWND(), LB_GETCURSEL, 0, 0);
  }

  std::vector<int> SelectedIndices(const ListBox& l) {
    assert(l.TheHWND() != nullptr);
    assert(HasStyle(l, LBS_MULTIPLESEL));

    int length = (int) SendMessage(l.TheHWND(), LB_GETSELCOUNT, 0, 0);
    if (length == LB_ERR) {
      // FIXME: should throw something more useful
      throw "LB_GETSELCOUNT failed.";
    }
    else if (length == 0) {
      return std::vector<int>();
    }

    std::vector<int> v(length, -1);
    SendMessage(l.TheHWND(), LB_GETSELITEMS, length, (LPARAM) &*v.begin());

    // Note: this might not be required
    //
    // I do this to ensure compatability with DeleteStrings which assumes that
    // indices are in ascending order. I *think* LB_GETSELITEMS does this anyway
    // but I can't find a guarentee in the docs. Since a call to sort
    // on most typical selection lists is likely to be cost-irrelevant
    // I figured this is a reasonable way to go. Your mileage may vary...
    //
    std::sort(begin(v), end(v), std::less<int>());

    return v;
  }

} // namespace jwt