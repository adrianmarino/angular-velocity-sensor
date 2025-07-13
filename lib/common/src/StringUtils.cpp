#include <StringUtils.h>

char *toCharArray(String str)
{
  char *char_array = new char[str.length() + 1];
  strcpy(char_array, str.c_str());
  return char_array;
}