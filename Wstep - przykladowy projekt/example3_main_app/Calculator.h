#ifndef CALCULATOR_H_
#define CALCULATOR_H_

namespace PROI_example3
{
class Calculator
{
public:
  Calculator();

  int getResult() const;

  void add(int i);

private:
  int accumulator;
};
}

#endif // CALCULATOR_H_
