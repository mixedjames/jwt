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
#include "message-pump.hpp"
#include <memory>
#include <algorithm>

std::unique_ptr<MessagePump> defaultPump_ = nullptr;

MessagePump::MessagePump()
  : dlgOrAccelChanged_(false)
{
}

int MessagePump::Pump() {
  MSG m;

  while (GetMessage(&m, nullptr, 0, 0)) {
    bool msgHandled = false;

    size_t l = dialogs_.size();
    for (size_t i = 0; i < l; ++i) {
      HWND d = dialogs_[i];
      if (d && IsDialogMessage(d, &m)) {
        msgHandled = true;
        break;
      }
    }

    if (!msgHandled) {
      l = accelerators_.size();
      for (size_t i = 0; i < l; ++i) {
        HACCEL a = accelerators_[i];
        if (a && TranslateAccelerator(m.hwnd, a, &m)) {
          msgHandled = true;
          break;
        }
      }
    }

    if (!msgHandled) {
      TranslateMessage(&m);
      DispatchMessage(&m);
    }

    if (dlgOrAccelChanged_) {
      dialogs_.erase(
        remove(begin(dialogs_), end(dialogs_), nullptr),
        end(dialogs_)
      );

      accelerators_.erase(
        remove(begin(accelerators_), end(accelerators_), nullptr),
        end(accelerators_)
      );

      dlgOrAccelChanged_ = false;
    }
  }
  return m.wParam;
}

void MessagePump::AddDialog(HWND h) {
  dialogs_.push_back(h);
  dlgOrAccelChanged_ = true;
}

void MessagePump::RemoveDialog(HWND h) {
  auto i = std::find(begin(dialogs_), end(dialogs_), h);

  if (i != dialogs_.end()) {
    *i = nullptr;
  }
  dlgOrAccelChanged_ = true;
}

void MessagePump::AddAccelerator(HACCEL h) {
  accelerators_.push_back(h);
  dlgOrAccelChanged_ = true;
}

void MessagePump::RemoveAccelerator(HACCEL h) {
  auto i = std::find(begin(accelerators_), end(accelerators_), h);

  if (i != accelerators_.end()) {
    *i = nullptr;
  }
  dlgOrAccelChanged_ = true;
}


MessagePump& DefaultPump() {
  if (!defaultPump_) {
    defaultPump_ = std::unique_ptr<MessagePump>(new MessagePump);
  }
  return *defaultPump_;
}