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

#include "ObjLexer.h"
#include <sstream>
#include <stdexcept>

/* obj file:

# comments

v  x y z
vt x y z
vn x y z

f v1 v2 v3

// TODO
f v1/vt1 v2/vt2 v3/vt3
f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3

*/

namespace obj_loader
{

bool isDigit(char const c)
{
  return (c >= '0' && c <= '9');
}

bool isLetter(char const c)
{
  return ((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z'));
}

bool isLiteral(char const c)
{
  return isLetter(c) || isDigit(c) || c == '-' || c == '.';
}

std::string getLiteral(int i, std::string const& raw_object)
{
  size_t end = i;

  while (end < raw_object.size() && isLiteral(raw_object[end]))
  {
    end++;
  }

  return raw_object.substr(i, end - i);
}

template<typename T>
T StringToNumber(std::string const& str)
{
  T num;

  std::stringstream ss(str);
  ss >> num;

  if (ss.fail())
  {
    std::runtime_error e(str);
    throw e;
  }

  return num;
}

bool IsNumber(std::string const& str)
{
  int start = 0;

  if (str[start] == '-')
    start++;

  return isDigit(str[start]);
}

int ObjLexer::AddNewToken(Token::Type const& type, int i, std::string const& raw_object)
{
  Token new_token;
  new_token.SetType(type);

  std::string str = getLiteral(i, raw_object);
  if (IsNumber(str))
    new_token.SetFloat(StringToNumber<float>(str));

  tokens_.push(new_token);

  return str.size();
}

ObjLexer::ObjLexer(std::string const& raw_object)
{
  char ch;
  int i = 0;

  while (i < (int)raw_object.size())
  {
    ch = raw_object[i];

    // v/vt/vn
    if (ch == 'v')
    {
      Token::Type type;

      if (raw_object[i + 1] == 't')
      {
        type = Token::Type::VERTEX_T;
      }
      else if (raw_object[i + 1] == 'n')
      {
        type = Token::Type::VERTEX_N;
      }
      else
      {
        type = Token::Type::VERTEX;
      }

      i += AddNewToken(type, i, raw_object);
    }
    // f
    else if (ch == 'f')
    {
      i += AddNewToken(Token::Type::FACE, i, raw_object);
    }
    // comment or g (grouping which we ignore atm)
    else if (ch == '#' || ch == 'g')
    {
      size_t size = raw_object.find('\n', i);
      i += size - i;
    }
    // everything else becomes a literal
    else if (isLiteral(ch))
    {
      i += AddNewToken(Token::Type::LITERAL, i, raw_object);
    }
    // whitespace, and eoln
    else if (ch == ' ' || ch == '\n')
    {
      i++;
      continue;
    }
  }
}

bool ObjLexer::HasNextToken() const
{
  return !tokens_.empty();
}

Token ObjLexer::NextToken()
{
  Token token = tokens_.front();
  tokens_.pop();

  return token;
}

} // namespace obj_loader
