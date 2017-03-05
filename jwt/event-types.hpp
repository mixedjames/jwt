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

typedef boost::signals2::scoped_connection Connection;

struct CloseTag {
};
extern const CloseTag Close;

struct CommandTag {
};
extern const CommandTag Command;

struct CommandEvent {
  enum Type {
    MENU = 1, ACCELERATOR, CONTROL
  } type;

  unsigned int id;
  LPARAM lParam;

  CommandEvent(Type t, unsigned int id, LPARAM l)
    : type(t), id(id), lParam(l)
  {}
};

struct ClickTag {
};
extern const ClickTag Click;