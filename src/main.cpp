#include <iostream>
#include <fstream>
#include <vector>

#include "Obj.h"
#include "ObjLexer.h"

std::string ReadInFile(char const* path)
{
  std::string str;
  std::ifstream is(path, std::ifstream::binary);

  if (is)
  {
    is.seekg(0, is.end);
    int length = (int)is.tellg();
    is.seekg(0, is.beg);

    char* buff = new char[length];

    is.read(buff, length);
    is.close();

    buff[length] = '\0';

    str = std::string(buff);
  }

  return str;
}

int main()
{
  std::string raw_object = ReadInFile("teapot.obj");
  Obj obj(raw_object);
}
