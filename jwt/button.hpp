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

/**
 * @file
 *
 * button.hpp contains three things:
 * 1. Definition for struct Button
 * 2. Definition for struct SplitButton
 * 3. Declarations for a series of non-member functions that manipulate
 *    Button instances through the public interface of Button & are therefore
 *    not members.
 */

namespace jwt {

  /**
   * Wraps the standard Button control.
   *
   * Usage
   * -----
   * ~~~~~~{.cpp}
   * // Create a new button from scratch:
   * Button b(parentWindow, L"My text");
   * SetBounds(b, Rect(0, 0, 150, 40));
   * 
   * // Use a button that's part of a dialog:
   * Button b(myDlg, IDC_MYBUTTON_ID);
   *
   * // Listen for events:
   * b.On(Click, []() { HandleTheClick(); });
   * ~~~~~~
   *
   * Important note
   * --------------
   * In general you probably don't need to use this class. Simple buttons in
   * dialogs where you just need to respond to clicks are probably better
   * handled by listening to the Command event on the parent Dialog. It's leaner
   * and easier to support accelerators etc.
   *
   * If you need to create a new button in memory & manipulate it then this
   * class if for you. Also, if you want to extend Button to handle
   * owner drawn behaviours or other notifications then this class may be for
   * you.
   *
   * Correctly extending Button
   * --------------------------
   * A minimal subclass of Button looks like this:
   * ~~~~~~{.cpp}
   * struct MyButton : Button {
   *   MyButton(Window& parent) : Button(defer_create) {
   *     Create(parent, 0);
   *   }
   * 
   * protected:
   *   MyButton(const defer_create_t&) : Button(defer_create) {}
   *
   *   LRESULT HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
   *     // Do custom processing here
   *     // ...
   *
   *     // Always forward messages to the superclass to ensure default
   *     // processing happens...
   *     return Button::HandleReflectedMessage(h, m, w, l);
   *   }
   * };
   * ~~~~~~
   */
  struct Button
    : Window
  {
    /**
     * Creates button with no text, whose bounds are (0,0,0,0).
     */
    Button(Window& parent);

    /**
     * Creates button with text, and a custom style. See MSDN button styles
     * for a list of options.
     */
    Button(Window& parent, const std::wstring&, DWORD buttonStyles = 0);

    /**
     * Creates button wrapper attached to a button within a dialog.
     * The buttonId specifies the control.
     */
    Button(Dialog& parent, int buttonId);

    /**
     * Register a click handler for this button.
     * See the notes above as to why you might not need to do this.
     */
    template<typename Callable>
    auto On(const ClickTag&, Callable c) -> decltype(onClick_.connect(c)) {
      return onClick_.connect(c);
    }

  protected:
    Button(const defer_create_t&);

    void Create(Window& parent, DWORD styles);

    LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
    boost::signals2::signal<void()> onClick_;
  };

// Split buttons only exist on Vista and later
//
#if (NTDDI_VERSION >= NTDDI_VISTA)

  /**
   * A split button is a standard button that has the BS_SPLITBUTTON style set.
   * This causes the button to have a second part within it that it intended
   * to bring up a context menu or perform some other secondary action.
   *
   * This class allows you to handle the secondary action.
   *
   * Example usage
   * -------------
   * ~~~~~~{.cpp}
   * SplitButton b(myDialog, IDC_MY_SPLIT_BUTTON_ID);
   * HMENU popupMenu;
   *
   * b.On(SecondaryAction, [&b, &popupMenu]() {
   *   ShowPopupMenuFor(b, popupMenu);
   * });
   * ~~~~~~
   */
  struct SplitButton
    : Button
  {
    SplitButton(Window& parent);
    SplitButton(Window& parent, const std::wstring&, DWORD buttonStyles = 0);
    SplitButton(Dialog& parent, int buttonId);

    using Button::On;

    template<typename Callable>
    auto On(const SecondaryActionTag&, Callable c)
      -> decltype(onDropdown_.connect(c))
    {
      return onDropdown_.connect(c);
    }

  protected:
    SplitButton(const defer_create_t&);

    LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
    boost::signals2::signal<void()> onDropdown_;
  };
#endif // Vista specific stuff

  /**
   * Loads an icon resource from the current module & associates it with
   * the specified button.
   */
  Button& SetIcon(Button&, WORD iconId);

// Split buttons & command links only exist on Vista and above.
//
#if (NTDDI_VERSION >= NTDDI_VISTA)

  /**
  * Shows a popup menu aligned correctly for a specified SplitButton.
  *
  * There are numerous steps requiring to correctly call TrackPopupMenuEx
  * so this call might be preferable to doing it yourself.
  */
  void ShowPopupMenuFor(SplitButton& b, HMENU menu);

  /**
  * Command links have two strings associated with them:
  * - The main text (set using the usual mechanism)
  * - A secondary note (set using BCM_SETNOTE)
  *
  * This function wraps BCM_SETNOTE.
  *
  * @return Button& allows chaining
  */
  Button& SetNote(Button& b, const std::wstring& note);

#endif
}