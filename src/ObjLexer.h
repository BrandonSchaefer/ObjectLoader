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

#ifndef OBJ_LEXER_H
#define OBJ_LEXER_H

#include <queue>
#include "Token.h"

namespace obj_loader
{

class ObjLexer
{
public:
  ObjLexer(std::string const& raw_object);

  bool HasNextToken() const;
  Token NextToken();

private:
  int AddNewToken(Token::Type const& type, int i, std::string const& raw_object);

  std::queue<Token> tokens_;
};

} // namespace obj_loader

#endif // OBJ_LEXER_H
