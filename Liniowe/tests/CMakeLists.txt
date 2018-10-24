find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(aisdiLinearTests test_main.cpp LinkedListTests.cpp VectorTests.cpp)
target_link_libraries(aisdiLinearTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun aisdiLinearTests)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      --build-config "$<CONFIGURATION>"
      DEPENDS aisdiLinearTests)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      DEPENDS aisdiLinearTests)
endif()
