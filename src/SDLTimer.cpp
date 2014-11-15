// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2014 Brandon Schaefer
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Brandon Schaefer <brandontschaefer@gmail.com>
 */

#include "SDLTimer.h"

#include <SDL2/SDL.h>

namespace obj_loader
{

SDLTimer::SDLTimer()
  : start_ticks_(0)
  , started_(false)
{
}

void SDLTimer::start()
{
  start_ticks_  = SDL_GetTicks();
  started_ = true;
}

unsigned SDLTimer::ticks() const
{
  unsigned ticks = 0;

  if (started_)
    ticks = SDL_GetTicks() - start_ticks_;

  return ticks;
}

bool SDLTimer::started() const
{
  return started_;
}

} // namespace obj_loader
