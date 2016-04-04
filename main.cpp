
#include <iostream>
#include "Envir.h"
#include "Cellule.h"

using std::cout;

int main() {

  srand(time(NULL));
  Envir A(1400,20);
  A.Run(1);
  return 0;
}
