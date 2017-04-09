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

namespace jwt {
  struct MessagePump {

    MessagePump();

    int Pump();

    void AddDialog(HWND);
    void RemoveDialog(HWND);

    void AddAccelerator(HACCEL);
    void RemoveAccelerator(HACCEL);

    void ReportException(std::exception_ptr);
    void RaiseReportedException();

  private:
    MessagePump(const MessagePump&) = delete;
    MessagePump& operator= (const MessagePump&) = delete;

    bool dlgOrAccelChanged_;
    std::vector<HWND> dialogs_;
    std::vector<HACCEL> accelerators_;
    std::exception_ptr currentException_;
  };

  MessagePump& DefaultPump();
}