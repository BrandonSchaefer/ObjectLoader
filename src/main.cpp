#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

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

int main()
{
}
