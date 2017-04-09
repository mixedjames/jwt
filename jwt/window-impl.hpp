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

namespace jwt {

  template<typename Callable>
  Window& ForEachChild(Window& w, Callable c) {
    struct Callback {
      Callable callback;
      HWND parent;
      std::exception_ptr reportedException;

      Callback(Callable c, HWND h) : callback(c), parent(h) {}
    } callback(c, w.TheHWND());

    WNDENUMPROC ep = [](HWND h, LPARAM l) {
      Callback* callback = (Callback*)l;

      if (GetAncestor(h, GA_PARENT) == callback->parent) {
        try {
          callback->callback(h);
        }
        catch (...) {
          callback->reportedException = std::current_exception();
          return FALSE;
        }
      }
      return TRUE;
    };

    EnumChildWindows(w.TheHWND(), ep, (LPARAM)&callback);
    if (callback.reportedException) {
      std::rethrow_exception(callback.reportedException);
    }

    return w;
  }

  template<typename Callable>
  Window& ForEachDescendant(Window&, Callable c) {
    struct Callback {
      Callable callback;
      std::exception_ptr reportedException;

      Callback(Callable c, HWND h) : callback(c) {}
    } callback(c);

    WNDENUMPROC ep = [](HWND h, LPARAM l) {
      Callback* callback = (Callback*)l;

      try {
        callback->callback(h);
      }
      catch (...) {
        callback->reportedException = std::current_exception();
        return FALSE;
      }

      return TRUE;
    };

    EnumChildWindows(w.TheHWND(), ep, (LPARAM)&callback);
    if (callback.reportedException) {
      std::rethrow_exception(callback.reportedException);
    }

    return w;
  }
}