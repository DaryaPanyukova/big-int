if(EXISTS "D:/ITMOlabs/basics_c++/labwork2-DaryaPanyukova/cmake-build-debug/tests/number_tests[1]_tests.cmake")
  include("D:/ITMOlabs/basics_c++/labwork2-DaryaPanyukova/cmake-build-debug/tests/number_tests[1]_tests.cmake")
else()
  add_test(number_tests_NOT_BUILT number_tests_NOT_BUILT)
endif()