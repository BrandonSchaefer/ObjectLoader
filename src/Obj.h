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

#ifndef OBJ_H
#define OBJ_H

#include <string>
#include <vector>

#include "ObjLexer.h"
#include "Face.h"
#include "Vertex.h"

namespace obj_loader
{

class Obj
{
public:
  Obj(std::string const& path);

private:
  void AddFace(ObjLexer& lexer);

  std::vector<Vertex> vs_;
  std::vector<Vertex> vts_;
  std::vector<Vertex> vns_;
  std::vector<Face>   faces_;

};

} // namespace obj_loader

#endif // OBJ_H
