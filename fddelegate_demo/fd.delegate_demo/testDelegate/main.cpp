#include "main.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"

void hello(int) { }

int main(int argc, char *argv[])
{
  test1();
  test2();
  test3();

  system("PAUSE");
  return EXIT_SUCCESS;
}
