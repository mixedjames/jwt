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
#include "measurement.hpp" 

/**
 * @file
 *
 * window.hpp contains two things:
 * 1. Definition for struct Window
 * 2. Declarations for a series of non-member functions that manipulate
 *    Window instances through the public interface of Window & are therefore not
 *    members.
 */

namespace jwt {

  /**
   *  Window is intended for use as a base-class for classes providing wrappers
   *  for specific window types: i.e. buttons, lists dialogs etc.
   *
   *  It provides several services to subclasses:
   *  - A common storage location for a target HWND (the protected hWnd_ member)
   *  - Reference-type behaviour (virtual destructor, deleted copies, protected
   *    construction)
   *  - Utility functions wrapping common functionality common to all HWNDs
   *    (sizing, text, styles etc.)
   *  - A mechanism for forwarding messages from child controls back to the
   *    specific Window instance that is wrapping that HWND.
   *
   *  Notes for subclass implementors
   *  ===============================
   *  If you simply wish to use pre-defined classes this information is unlikely
   *  to be helpful so I suggest you stop here.
   *  
   *  If you wish to build your own custom controls or write a wrapper for a
   *  control that I have not provided then read on.
   *
   *  Construction
   *  ------------
   *  Window provides only a single constructor - the default one. This is
   *  intentional. I recognised during development that many specific Window
   *  types do have a valid HWND until they are part way through their own
   *  constructor.
   *
   *  Subclasses should ensure they set the hWnd_ member as soon as they have
   *  access to it. This may be different depending on what sort of class you
   *  are writing:
   *
   *  - Wrapper for pre-existing control: HWND is available as soon as
   *    CreateWindow(Ex) returns but not before.
   *  - Custom control built specifically with JWT: HWND is available as soon as
   *    the WM_NCCREATE message is processed; see CustomWindow for details.
   *
   *  Message reflection
   *  ------------------
   *  The Windows API forwards notification messages from controls to the parent
   *  window of that control. i.e. a button sends messages to its parent dialog
   *  etc.
   *
   *  If you are writing a wrapper for a control that's not very helpful - we
   *  want to process that message within the wrapper itself, not the parent
   *  window. Window provides a mechanism to do this in a standard way - I call
   *  this message reflection.
   *
   *  ### How does it work? ###
   *
   *  Any class that is a parent of other windows (see AppWindow, Dialog etc.)
   *  should look for notification messages in its WndProc:
   *  - WM_COMMAND
   *  - WM_NOTIFY
   *  - WM_HSCROLL/WM_VSCROLL
   *
   *  When it identifies one if these messages it should call
   *  `Window::ReflectMessage(...)` copying the message parameters directly to
   *  this function.
   *
   *  This function will examine the message in question and, if it finds a
   *  Window, it calls the virtual function `HandleReflectedMessage(...)` for
   *  that Window. A wrapper subclass should override this function and handle
   *  these messages accordingly. 
   */
  struct Window {

    virtual ~Window() {}

    /**
     * Gets the HWND contained by this Window.
     * @return HWND
     */
    HWND TheHWND() { return hWnd_; }

    /**
     * Gets a const version of the HWND contained by this Window.
     * @return const HWND
     */
    const HWND TheHWND() const { return hWnd_; }

  protected:
    HWND hWnd_;

    /**
     * Default constructor. Sets hWnd_ to nullptr.
     */
    Window() : hWnd_(nullptr) {}

    /**
     * This method is part of the message reflection mechanism.
     *
     * Classes that may have child windows should call this function from
     * their WndProc whenever they receive a notification message. This function
     * will then dispatch the message appropriately to the wrapper class for
     * the child window.
     *
     * At the very least, parent windows should ensure they forward:
     * - WM_COMMAND
     * - WM_NOTIFY
     * - WM_HSCROLL
     * - WM_VSCROLL
     *
     * Messages should be altered unchanged.
     *
     * @return FALSE if the the child window requests default processing
     *         take place. A non-FALSE value otherwise - typically this should
     *         be returned directly by the parent's WndProc.
     */
    static LRESULT ReflectMessage(HWND, UINT, WPARAM, LPARAM);

    /**
     * JWT controls should override this method to receive notification
     * messages from their underlying HWND.
     *
     * Note that because this might be called from a WndProc or a DlgProc
     * you should return FALSE if you want the default processing to occur.
     * The parent window will ensure that this happens; you must not call
     * DefWindowProc yourself.
     *
     * For example, the Button class might implement this function as follows:
     * ~~~~~~~~~~~~{.cpp}
     * LRESULT HandleReflectedMessage(HWND h, UINT m, WPARAM w, LPARAM l) {
     *   switch (m) {
     *     case WM_COMMAND:
     *       if (HIWORD(w) == BN_CLICKED) {
     *         IWasClicked();
     *       }
     *       break;
     *   }
     *   return FALSE;
     * }
     * ~~~~~~~~~~~~
     */
    virtual LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
  };

  /**
   * Gets the outer size of a Window. (i.e. the size measured outside of the
   * non-client area)
   * @return Dimension
   */
  Dimension Size(const Window&);

  /**
   * Sets the outer size of a Window. (i.e. the size measured outside of the
   * non-client area)
   *
   * @return Window& - the Window on which the function was called; allows
   *         chaining.
   */
  Window& Size(Window&, const Dimension&);

  /**
   * Gets the client size of a Window. (i.e. the size measured inside of the
   * non-client area).
   * @return Dimension
   */
  Dimension ClientSize(const Window&);

  /**
   * Sets the client size of a Window. (i.e. the size measured inside of the
   * non-client area). The total size of the Window will be the client size
   * plus the size of the non-client area.
   *
   * @return Window& - the Window on which the function was called; allows
   *         chaining.
   */
  Window& ClientSize(Window&, const Dimension&);

  /**
   * Sets the position of a Window. The units depend on the type of Window:
   * - Child window: local coordinates of the parent window
   * - Non-child window: screen coordinates
   *
   * Note that in either case the coordinates may be negative.
   * @return Point
   */
  Point Position(const Window&);

  /**
   * Gets the position of a Window. The units depend on the type of Window:
   * - Child window: local coordinates of the parent window
   * - Non-child window: screen coordinates
   *
   * Note that in either case the coordinates may be negative.
   *
   * @return Window& - the Window on which the function was called; allows
   *         chaining.
   */
  Window& Position(Window&, const Point&);

  Rect Bounds(const Window&);
  Window& Bounds(Window&, const Rect&);

  inline DWORD Style(const Window& w) {
    return GetWindowLong(w.TheHWND(), GWL_STYLE);
  }

  inline DWORD ExStyle(const Window& w) {
    return GetWindowLong(w.TheHWND(), GWL_EXSTYLE);
  }

  std::wstring Text(const Window&);
  Window& Text(Window&, const std::wstring&);

  bool Visible(const Window&);
  Window& Visible(Window&, bool);

  /**
   * Iterates through all the child windows of this window and calls c for
   * each one with the child HWND as an argument.
   *
   * This function is analagous to `EnumChildWindow` but has subtly different
   * behaviour: `EnumChildWindow` returns every descendant (i.e. children,
   * grandchildren, ...) whereas ForEachChild returns *only* direct children.
   */
  template<typename Callable>
  Window& ForEachChild(Window&, Callable c);

  /**
   * Iterates through all the descendant windows of this window and calls c for
   * each one with the child HWND as an argument.
   *
   * This function is truly analagous to `EnumChildWindow` unlike `ForEachChild`
  */
  template<typename Callable>
  Window& ForEachDescendant(Window&, Callable c);
}

#include "window-impl.hpp"