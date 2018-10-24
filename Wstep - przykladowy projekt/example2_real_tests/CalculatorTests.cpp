#include <boost/test/unit_test.hpp>

#include "Calculator.h"

using namespace PROI_example2;

BOOST_AUTO_TEST_SUITE(CalculatorTests)

BOOST_AUTO_TEST_CASE(constructed_Calculator_returns_zero)
{
  Calculator c;
  BOOST_CHECK_EQUAL(c.getResult(), 0);
}

BOOST_AUTO_TEST_CASE(after_single_add_result_is_modified)
{
  Calculator c;
  c.add(1);
  BOOST_CHECK_EQUAL(c.getResult(), 1);
}

BOOST_AUTO_TEST_CASE(multiple_additions_result_in_sum)
{
  Calculator c;
  c.add(2);
  c.add(2);
  BOOST_CHECK_EQUAL(c.getResult(), 4);
}

BOOST_AUTO_TEST_SUITE_END()
