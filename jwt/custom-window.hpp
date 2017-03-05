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

#include <Windows.h>
#include <assert.h>
#include "window.hpp"

template<typename UniqueTag>
struct CustomWindow
  : Window
{
protected:
  CustomWindow() {}
  virtual ~CustomWindow();

  static void Register(const wchar_t* clsName);

  virtual LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

private:
  static ATOM atom_;

  CustomWindow(const CustomWindow&) = delete;
  CustomWindow& operator= (const CustomWindow&) = delete;

  static LRESULT CALLBACK WndProcAdapter(HWND, UINT, WPARAM, LPARAM);
  virtual LRESULT PrivateWndProc(HWND, UINT, WPARAM, LPARAM);
};

#include "custom-window-impl.hpp"