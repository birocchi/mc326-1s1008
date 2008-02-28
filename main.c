#include "io.h"
#include "parser.h"

int main(int argc, char* argv[])
{
  int halt = 0;

  printHeader();

  while (!halt) {
    printMenu();
    halt = parseCommand();
  }

  return 0;
}
