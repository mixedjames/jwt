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

namespace jwt {

  struct Window {

    virtual ~Window() {}

    explicit operator HWND() { return hWnd_; }

    Dimension Size() const;
    Window& Size(const Dimension&);

    Dimension ClientSize() const;
    Window& ClientSize(const Dimension&);

    int Style() const { return GetWindowLong(hWnd_, GWL_STYLE); }
    int ExStyle() const { return GetWindowLong(hWnd_, GWL_EXSTYLE); }

    std::wstring Text() const;
    Window& Text(const std::wstring&);

    bool Visible() const;
    Window& Visible(bool);

    template<typename Callable>
    Window& ForEachChild(Callable c);

  protected:
    HWND hWnd_;

    Window() : hWnd_(nullptr) {}

    static LRESULT ReflectMessage(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT HandleReflectedMessage(HWND, UINT, WPARAM, LPARAM);

  private:
    Window(const Window&) = delete;
    Window& operator= (const Window&) = delete;
  };

}

#include "window-impl.hpp"