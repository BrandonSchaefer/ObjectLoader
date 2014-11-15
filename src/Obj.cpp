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

void Obj::AddVertex(ObjLexer& lexer, std::vector<Vertex>& vertex_list)
{
  float p1 = lexer.NextToken().GetFloat();
  float p2 = lexer.NextToken().GetFloat();
  float p3 = lexer.NextToken().GetFloat();

  if (p1 > max_vertex_)
    max_vertex_ = p1;

  if (p2 > max_vertex_)
    max_vertex_ = p2;

  if (p3 > max_vertex_)
    max_vertex_ = p3;

  Vertex v(p1, p2, p3);
  vertex_list.push_back(v);
}

int Obj::NumOfFaces() const
{
  return faces_.size();
}

float Obj::MaxVertex() const
{
  return max_vertex_;
}

GLfloat* Obj::RawFaces() const
{
  return raw_faces_;
}

void Obj::GenerateRawFaces()
{
  int num_faces = (int)faces_.size();
  int new_i = 0;
  raw_faces_ = (GLfloat*)calloc(1, sizeof(GLfloat) * num_faces * 9);

  for (int i = 0; i < num_faces; i++)
  {
    Face face = faces_[i];

    new_i = i * 9;
    raw_faces_[0 + new_i] = face.v1.x;
    raw_faces_[1 + new_i] = face.v1.y;
    raw_faces_[2 + new_i] = face.v1.z;

    raw_faces_[3 + new_i] = face.v2.x;
    raw_faces_[4 + new_i] = face.v2.y;
    raw_faces_[5 + new_i] = face.v2.z;

    raw_faces_[6 + new_i] = face.v3.x;
    raw_faces_[7 + new_i] = face.v3.y;
    raw_faces_[8 + new_i] = face.v3.z;
  }
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
  : raw_faces_(nullptr)
  , max_vertex_(0.0f)
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

  GenerateRawFaces();
}

Obj::~Obj()
{
  free(raw_faces_);
}

} // namespace obj_loader
