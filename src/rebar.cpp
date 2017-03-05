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

#include "rebar.hpp"

Rebar::Rebar(Window& parent) {
  Create(parent);
}

Rebar::Rebar(const defer_create_t&) {

}

Rebar& Rebar::AddBand(Window& w) {
  assert(hWnd_ != nullptr);
  assert(HWND(w) != nullptr);

  REBARBANDINFO rbbi = {};

  rbbi.cbSize = sizeof(REBARBANDINFO);
  rbbi.fMask = RBBIM_STYLE | RBBIM_TEXT | RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE;
  rbbi.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS | RBBS_VARIABLEHEIGHT;

  rbbi.lpText = L"Test";
  rbbi.hwndChild = (HWND) w;
  rbbi.cyChild = 50;
  rbbi.cxMinChild = 100;
  rbbi.cyMinChild = 50;
  rbbi.cx = 100;

  SendMessage(hWnd_, RB_INSERTBAND, -1, (LPARAM) &rbbi);

  return *this;
}

void Rebar::Create(Window& parent) {
  hWnd_ = CreateWindow(
    REBARCLASSNAME, nullptr,
    WS_VISIBLE | WS_CHILD | RBS_AUTOSIZE,
    0, 0, 0, 0,
    (HWND)parent, nullptr, HINST_COMMCTRL, nullptr
  );

  assert(hWnd_ != nullptr);
}

LRESULT Rebar::HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM) {
  return FALSE;
}
