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

#include "defer-create.hpp"
#include "custom-window.hpp"
#include "event-types.hpp"

namespace jwt {

  struct AppWindow
    : CustomWindow<AppWindow>
  {
    friend struct CustomWindow<AppWindow>;
    static const wchar_t* CLASS_NAME;

    static void Register();

    AppWindow();

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

    template<typename Callable>
    void SizePolicy(Callable c) {
      sizePolicy_ = c;
    }

    template<typename Callable>
    void LayoutPolicy(Callable c) {
      layoutPolicy_ = c;
    }

    AppWindow& Menu(UINT resource);

  protected:
    AppWindow(const defer_create_t&);

    void Create();

    LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);

  private:
    boost::signals2::signal<void()> onClose_;
    boost::signals2::signal<void(const CommandEvent&)> onCommand_;
    std::function<void(unsigned int edge, Rect&)> sizePolicy_;
    std::function<void()> layoutPolicy_;
  };

}