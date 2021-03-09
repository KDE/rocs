# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMMarkAsTest)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)

if(NOT Qt5Test_FOUND)
    message(STATUS "Qt5Test not found, autotests will not be built.")
    return()
endif()

macro(GRAPHTHEORY_UNIT_TESTS)
   foreach(_testname ${ARGN})
      add_executable(${_testname} ${_testname}.cpp)
      add_test(NAME graphtheory-${_testname} COMMAND ${_testname})
      target_link_libraries(${_testname} rocsgraphtheory Qt5::Test Qt5::XmlPatterns )
      ecm_mark_as_test(${_testname})
   endforeach()
endmacro()

graphtheory_unit_tests(
   test_graphoperations
   test_kernel
   test_kernelscriptapi
)
