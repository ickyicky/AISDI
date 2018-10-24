#include "Calculator.h"

using namespace PROI_example2;

Calculator::Calculator()
  : accumulator(0)
{}

int Calculator::getResult() const
{
  return accumulator;
}

void Calculator::add(int i)
{
  accumulator += i;
}
