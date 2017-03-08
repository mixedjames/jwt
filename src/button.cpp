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
#include "button.hpp"
#include <assert.h>

namespace jwt {

  //
  // **************************************************
  // Button member function definitions
  // **************************************************
  //

  Button::Button(Window& parent) {
    Create(parent, 0);
  }

  Button::Button(Window& parent, const std::wstring& txt, DWORD buttonStyles) {
    Create(parent, buttonStyles);
    SetText(*this, txt);
  }

  Button::Button(Dialog& parent, int buttonId) {
    hWnd_ = parent.Item(buttonId);
    
    assert(hWnd_ != nullptr);
    auto s = ClassName(*this);
    assert(ClassName(*this) == L"Button");

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  Button::Button(const defer_create_t&) {
  }

  void Button::Create(Window& parent, DWORD styles) {
    hWnd_ = CreateWindow(
      L"BUTTON", L"", WS_VISIBLE | WS_CHILD | styles,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT,
      parent.TheHWND(), nullptr, nullptr, nullptr
    );
    assert(hWnd_ != nullptr);

    SetWindowLongPtr(hWnd_, GWL_USERDATA, (LONG_PTR) this);
  }

  LRESULT Button::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_COMMAND:
      if (HIWORD(w) == BN_CLICKED) {
        onClick_();
        return 0;
      }
      break;
    }

    return 0;
  }

  //
  // **************************************************
  // SplitButton member function definitions
  // **************************************************
  //
#if (NTDDI_VERSION >= NTDDI_VISTA)

  SplitButton::SplitButton(Window& parent)
    : Button(defer_create)
  {
    Create(parent, 0);
  }

  SplitButton::SplitButton(Window& parent, const std::wstring& txt, DWORD buttonStyles)
    : Button(defer_create)
  {
    Create(parent, BS_SPLITBUTTON | buttonStyles);
    SetText(*this, txt);
  }

  SplitButton::SplitButton(Dialog& parent, int buttonId)
    : Button(parent, buttonId)
  {
    assert(HasStyle(*this, BS_SPLITBUTTON));
  }

  SplitButton::SplitButton(const defer_create_t&)
    : Button(defer_create)
  {
  }

  LRESULT SplitButton::HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
    if (m == WM_NOTIFY) {
      NMBCDROPDOWN* n = (NMBCDROPDOWN*) l;

      if (n->hdr.code == BCN_DROPDOWN) {
        onDropdown_();
      }
    }

    return Button::HandleReflectedMessage(h, m, w, l);
  }

  //
  // **************************************************
  // Non-member Button function definitions
  // **************************************************
  //

  Button& SetIcon(Button& b, WORD iconId) {
    HICON icon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(iconId));
    SendMessage(b.TheHWND(), BM_SETIMAGE, IMAGE_ICON, (LPARAM) icon);
    return b;
  }

  void ShowPopupMenuFor(SplitButton& b, HMENU menu) {
    assert(b.TheHWND() != nullptr);
    assert(menu != nullptr);

    HWND parent = GetAncestor(b.TheHWND(), GA_PARENT);

    // Step 1: Calculate the correct exclusion rect to pass to
    //         TrackPopupMenuEx. This is specified in screen coords
    //         so can't use jwt::GetBounds.
    //
    RECT bounds;
    GetWindowRect(b.TheHWND(), &bounds);

    TPMPARAMS tp = {
      sizeof(TPMPARAMS),
      bounds
    };

    // Step 2: figure out which way the menu is supposed to align.
    //         This is a system property & varies between Windows configs.
    //
    UINT flags = TPM_TOPALIGN;
    if (GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0) {
      // Left align
      flags |= TPM_LEFTALIGN;
    }
    else {
      // Right align menus
      flags |= TPM_RIGHTALIGN;
    }

    // Step 3: show the menu
    TrackPopupMenuEx(
      menu, flags,
      bounds.right, bounds.bottom,
      parent, &tp
    );
  }

  Button& SetNote(Button& b, const std::wstring& note) {
    assert(HasStyle(b, BS_COMMANDLINK));
    SendMessage(b.TheHWND(), BCM_SETNOTE, 0, (LPARAM)note.c_str());
    return b;
  }

#endif

} // namespace jwt