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

namespace jwt {
  struct Dimension {
    int w;
    int h;

    Dimension() : w(0), h(0) {}
    Dimension(int w, int h) : w(w), h(h) {}
    explicit Dimension(const RECT& r) : w(r.right - r.left), h(r.bottom - r.top) {}

    explicit operator RECT() const {
      RECT r = {
        0, 0, w, h
      };
      return r;
    }
  };

  struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    explicit Point(const POINT& p) : x(p.x), y(p.y) {}

    explicit operator POINT() const {
      POINT p = {
        x, y
      };
      return p;
    }
  };

  struct Rect {
    Point position;
    Dimension size;

    Rect() {}
    Rect(Point p, Dimension s) : position(p), size(s) {}
    Rect(int x, int y, int w, int h) : position(x, y), size(w, h) {}
    explicit Rect(const RECT& r) : position(r.left, r.top), size(r) {}

    explicit operator RECT() const {
      RECT r = {
        position.x, position.y,
        position.x + size.w, position.y + size.h
      };
      return r;
    }
  };
}