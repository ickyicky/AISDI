PROI Example Project(s)
========================

Small sub-projects included in this project are intended to show,
how one can write (semi-)portable C++ programs,
with close to 100% test coverage.

Requirements
--------------
 * C++11 compatible compiler (g++ 4.7/4.8)
 * GNU make
 * Boost Libraries (Unit Test Framework)

Building
--------------
All sub-projects contains proper Makefiles.
To build single sub-project, enter it's directory and type `make`.

Sub-Projects
--------------

### example1_basics
Project shows how to use Makefile and Boost Unit Test Framework.
Basic Makefile dependencies and test unit/suite are shown.

### example2_real_tests
Project presents more complex Makefile and include 'real' Calculator class,
that perfoms basic calculations and is tested by complete unit tests suite.

### example3_main_app
Project presents how to run tests and provide standalone application.
'Library' contains logic and is tested, 'main.cpp' should only
connect library with user input.