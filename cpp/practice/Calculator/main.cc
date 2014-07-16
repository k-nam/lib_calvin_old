#include "Desk_Calculator.h"

using namespace lib_calvin;

void main() {
  Desk_Calculator calculator;
  calculator.Calculate("123 + (23+23*(123 + 23.142))");
}
