//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/* * Copyright (C) CURRENT_YEAR Brandon Schaefer
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

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
public:
  enum class Type : unsigned int
  {
    LITERAL,
    VERTEX,
    VERTEX_T,
    VERTEX_N,
    FACE,
    Size
  };

  Token();

  void SetType(Type const& type);
  void SetFloat(float value);

  Type GetType() const;
  float GetFloat() const;

private:
  float value_;
  Type type_;
};

#endif // TOKEN_H
