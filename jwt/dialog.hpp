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
#include "defer-create.hpp"
#include "event-types.hpp"

struct Dialog
  : Window
{
  Dialog(int resourceId);
  Dialog(Window& parent, int resourceId);
  ~Dialog();

  HWND Item(int id);

  template<typename Callable>
  auto On(const CloseTag&, Callable c) -> decltype(onClose_.connect(c)) {
    return onClose_.connect(c);
  }

  template<typename Callable>
  auto On(const CommandTag&, Callable c) -> decltype(onCommand_.connect(c)) {
    return onCommand_.connect(c);
  }

  template<typename Callable>
  auto On(const CommandTag&, int id, Callable c) -> decltype(onCommand_.connect(c)) {
    return onCommand_.connect([c, id](const CommandEvent& e) {
      if (e.id == id) {
        c();
      }
    });
  }

protected:
  Dialog(const defer_create_t&);

  void Create(int resourceId);
  void Create(Window& parent, int resourceId);

  virtual INT_PTR DlgProc(HWND, UINT, WPARAM, LPARAM);

private:
  boost::signals2::signal<void()> onClose_;
  boost::signals2::signal<void(const CommandEvent&)> onCommand_;

  INT_PTR PrivateDlgProc(HWND, UINT, WPARAM, LPARAM);
  static INT_PTR CALLBACK DlgProcAdapter(HWND, UINT, WPARAM, LPARAM);
};