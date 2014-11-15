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

#include "Obj.h"
#include "Token.h"

namespace obj_loader
{

void AddVertex(ObjLexer& lexer, std::vector<Vertex>& vertex_list)
{
  Token p1 = lexer.NextToken();
  Token p2 = lexer.NextToken();
  Token p3 = lexer.NextToken();

  Vertex v(p1.GetFloat(), p2.GetFloat(), p3.GetFloat());
  vertex_list.push_back(v);
}

void Obj::AddFace(ObjLexer& lexer)
{
  size_t i1 = (size_t)lexer.NextToken().GetFloat();
  size_t i2 = (size_t)lexer.NextToken().GetFloat();
  size_t i3 = (size_t)lexer.NextToken().GetFloat();

  // TODO add a check somewhere to see if the index starts at 0 or 1.
  // Standard says its starts at 1, but some start at 0.
  i1--;
  i2--;
  i3--;

  // Sanity check
  if (i1 <= vs_.size() &&
      i2 <= vs_.size() &&
      i3 <= vs_.size())
  {
    Vertex v1, v2, v3;
    v1 = vs_[i1];
    v2 = vs_[i2];
    v3 = vs_[i3];

    Face face(vs_[i1], vs_[i2], vs_[i3]);
    faces_.push_back(face);
  }
}

Obj::Obj(std::string const& raw_object)
{
  ObjLexer lexer(raw_object);

  while (lexer.HasNextToken())
  {
    Token token = lexer.NextToken();
    switch (token.GetType())
    {
      case (Token::Type::VERTEX):
        AddVertex(lexer, vs_);
        break;
      case (Token::Type::VERTEX_T):
        AddVertex(lexer, vts_);
        break;
      case (Token::Type::VERTEX_N):
        AddVertex(lexer, vns_);
        break;
      case (Token::Type::FACE):
        AddFace(lexer);
        break;
      default:
       break;
    }
  }
}

} // namespace obj_loader
