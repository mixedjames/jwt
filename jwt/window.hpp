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
  @file

  window.hpp contains two things:
  1. Definition for struct Window
  2. Declarations for a series of non-member functions that manipulate
     Window instances through the public interface of Window & are therefore not
     members.
*/

namespace jwt {

  /**
     Window is intended for use as a base-class for classes providing wrappers
     for specific window types: i.e. buttons, lists dialogs etc.

     It provides several services to subclasses:
     - A common storage location for a target HWND (the protected hWnd_ member)
     - Reference-type behaviour (virtual destructor, deleted copies, protected construction)
     - Utility functions wrapping common functionality common to all HWNDs (sizing, text, styles etc.)
     - A mechanism for forwarding messages from child controls back to the specific
       Window instance that is wrapping that HWND.

     Notes for subclass implementors
     ===============================
     If you simply wish to use pre-defined classes this information is unlikely to be helpful
     so I suggest you stop here.
     
     If you wish to build your own custom controls or write a wrapper for a control that I
     have not provided then read on.

     Construction
     ------------
     Window provides only a single constructor - the default one. This is intentional. I
     recognised during development that many specific Window types do have a valid HWND
     until they are part way through their own constructor.

     Subclasses should ensure they set the hWnd_ member as soon as they have access to it.
     This may be different depending on what sort of class you are writing:
     
     - Wrapper for pre-existing control: HWND is available as soon as CreateWindow(Ex)
       returns but not before.
     - Custom control built specifically with JWT: HWND is available as soon as the 
       WM_NCCREATE message is processed; see CustomWindow for details.

     Message reflection
     ------------------
     The Windows API forwards notification messages from controls to the parent window
     of that control. i.e. a button sends messages to its parent dialog etc.

     If you are writing a wrapper for a control that's not very helpful - we want to
     process that message within the wrapper itself, not the parent window. Window
     provides a mechanism to do this in a standard way - I call this message reflection.

     ### How does it work? ###

     Any class that is a parent of other windows (see AppWindow, Dialog etc.) should look
     for notification messages in its WndProc:
     - WM_COMMAND
     - WM_NOTIFY
     - WM_HSCROLL/WM_VSCROLL

     When it identifies one if these messages it should call `Window::ReflectMessage(...)` copying
     the message parameters directly to this function.

     This function will examine the message in question and, if it finds a Window, it calls
     the virtual function `HandleReflectedMessage(...)` for that Window. A wrapper subclass should
     override this function and handle these messages accordingly. 
   */
  struct Window {

    virtual ~Window() {}

    HWND TheHWND() { return hWnd_; }
    const HWND TheHWND() const { return hWnd_; }

  protected:
    HWND hWnd_;

    Window() : hWnd_(nullptr) {}

    static LRESULT ReflectMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
  };

  Dimension Size(const Window&);
  Window& Size(Window&, const Dimension&);

  Dimension ClientSize(const Window&);
  Window& ClientSize(Window&, const Dimension&);

  Point Position(const Window&);
  Window& Position(Window&, const Point&);

  Rect Bounds(const Window&);
  Window& Bounds(Window&, const Rect&);

  inline DWORD Style(const Window& w) { return GetWindowLong(w.TheHWND(), GWL_STYLE); }
  inline DWORD ExStyle(const Window& w) { return GetWindowLong(w.TheHWND(), GWL_EXSTYLE); }

  std::wstring Text(const Window&);
  Window& Text(Window&, const std::wstring&);

  bool Visible(const Window&);
  Window& Visible(Window&, bool);

  /**
    Iterates through all the child windows of this window and calls c for
    each one with the child HWND as an argument.

    This function is analagous to `EnumChildWindow` but has subtly different behaviour:
    `EnumChildWindow` returns every descendant (i.e. children, grandchildren, ...)
    whereas ForEachChild returns *only* direct children.
  */
  template<typename Callable>
  Window& ForEachChild(Window&, Callable c);

  /**
    Iterates through all the descendant windows of this window and calls c for
    each one with the child HWND as an argument.

    This function is truly analagous to `EnumChildWindow` unlike `ForEachChild`
  */
  template<typename Callable>
  Window& ForEachDescendant(Window&, Callable c);
}

#include "window-impl.hpp"