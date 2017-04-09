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
#include "toolbar.hpp"

namespace jwt {

  Toolbar::Toolbar(Window& parent) {
    Create(parent);
  }

  Toolbar::Toolbar(const defer_create_t&) {
  }

  int Toolbar::AddStandardBitmap(UINT id) {
    TBADDBITMAP addBmp = {
      HINST_COMMCTRL,
      id
    };

    LRESULT index = SendMessage(hWnd_, TB_ADDBITMAP, 0, (WPARAM)&addBmp);
    assert(index != -1);

    return (int) index;
  }

  int Toolbar::AddBitmapResource(UINT id) {
    TBADDBITMAP addBmp = {
      GetModuleHandle(nullptr),
      id
    };

    int index = (int) SendMessage(hWnd_, TB_ADDBITMAP, 0, (WPARAM)&addBmp);
    assert(index != -1);

    return index;
  }


  Toolbar& Toolbar::AddButton(int baseImgIndex, int subImgIndex, int id, const std::wstring& label) {
    TBBUTTON b = {
      MAKELONG(subImgIndex, baseImgIndex),
      id,
      TBSTATE_ENABLED,
      BTNS_AUTOSIZE,
      { 0 }, 0,
      (INT_PTR)label.c_str()
    };

    SendMessage(hWnd_, TB_ADDBUTTONS, 1, (LPARAM)&b);

    return *this;
  }

  Toolbar& Toolbar::AddSeparator() {
    TBBUTTON b = {
      MAKELONG(0, 0),
      NULL,
      0,
      TBSTYLE_SEP,
      {},
      0,
      (INT_PTR)L""
    };
    SendMessage(hWnd_, TB_ADDBUTTONS, 1, (LPARAM)&b);

    return *this;
  }

  Toolbar& Toolbar::Autosize() {
    SendMessage(hWnd_, TB_AUTOSIZE, 0, 0);
    return *this;
  }

  void Toolbar::Create(Window& parent) {
    hWnd_ = CreateWindowEx(0, TOOLBARCLASSNAME, 0,
      TBSTYLE_FLAT | CCS_ADJUSTABLE | CCS_NODIVIDER | WS_CHILD | WS_VISIBLE | CCS_NOPARENTALIGN | CCS_NORESIZE,
      0, 0, 0, 0,
      parent.TheHWND(), (HMENU)0, GetModuleHandle(nullptr), 0);

    SendMessage(hWnd_, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
  }

  LRESULT Toolbar::HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM) {
    return 0;
  }

} // namespace jwt