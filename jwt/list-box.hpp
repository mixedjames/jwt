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

#include "libraries.hpp"
#include "window.hpp"
#include "dialog.hpp"
#include "defer-create.hpp"
#include "event-types.hpp"

namespace jwt {

  struct ListBox
    : Window
  {
    ListBox(Window& parent);
    ListBox(Dialog& parent, int buttonId);

  protected:
    ListBox(const defer_create_t&);

    void Create(Window& parent);

    LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
  };

  ListBox& AddString(ListBox&, const std::wstring&);
  ListBox& InsertString(ListBox&, int index, const std::wstring&);

  ListBox& DeleteString(ListBox&, int index);

  template<typename Container>
  ListBox& DeleteStrings(ListBox& l, const Container& indices);

  std::wstring GetString(ListBox&, int index);


  int Count(ListBox&);

  int SelectedIndex(const ListBox&);
  std::vector<int> SelectedIndices(const ListBox&);
}

#include "list-box-impl.hpp"