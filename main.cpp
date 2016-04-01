
#include <iostream>
#include "Envir.h"
#include "Cellule.h"

using std::cout;

int main() {
  srand(time(NULL));
  Envir A(200,30);
  A.Run(5);
  return 0;
}
