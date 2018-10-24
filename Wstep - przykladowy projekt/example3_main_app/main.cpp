#include <iostream>

#include "Calculator.h"

using PROI_example3::Calculator;

int main()
{
  Calculator calc;
  // This is NOT perfect user input validation...
  // Also - this could be extracted to separate, tested class
  while (std::cin)
  {
    std::cout << "Enter integer (or any character to exit): ";
    int i = 0;
    std::cin >> i;
    calc.add(i);
  }
  std::cout << "Result: " << calc.getResult() << std::endl;
}
