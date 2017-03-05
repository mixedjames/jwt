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

#include "custom-window.hpp"
#include "defer-create.hpp"

struct ScrollPane
  : CustomWindow<ScrollPane>
{
  friend struct CustomWindow<ScrollPane>;
  static const wchar_t* CLASS_NAME;

  static void Register();

  explicit ScrollPane(Window& parent);

  ScrollPane& CalculateScrollableExtent();
  const Point& Position() const { return position_; }
  const Dimension& Extent() const { return extent_; }

  bool AlwaysOn() const { return !!(flags_ & ALWAYS_ON); }
  ScrollPane& AlwaysOn(bool b);

protected:
  explicit ScrollPane(const defer_create_t&);

  void Create(Window& parent);
  LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

private:
  enum Flags {
    ALWAYS_ON = 0x01
  };

  Dimension extent_;
  Point position_;
  unsigned int flags_;
  
  void ConfigScrollbars();
  void ConfigOptionalScrollbars();
  void ConfigAlwaysOnScrollbars();
  void HandleHScroll(int action);
  void HandleVScroll(int action);
};