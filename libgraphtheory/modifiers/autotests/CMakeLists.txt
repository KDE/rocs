#  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
#
#  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

#Gets graphs for the data-driven functionality test
file(COPY graphs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(testforcebasedlayout_SRCS
    testforcebasedlayout.cpp
)


add_executable(TestForceBasedLayout ${testforcebasedlayout_SRCS})
add_test(NAME TestForceBasedLayout COMMAND TestForceBasedLayout)
ecm_mark_as_test(TestForceBasedLayout)
target_link_libraries(TestForceBasedLayout
    rocsgraphtheory
    Qt5::Test
)
target_compile_definitions(TestForceBasedLayout PRIVATE ADDITIONAL_PLUGIN_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(testradiallayout_SRCS
    testradiallayout.cpp
)


add_executable(TestRadialLayout ${testradiallayout_SRCS})
add_test(NAME TestRadialLayout COMMAND TestRadialLayout)
ecm_mark_as_test(TestRadialLayout)
target_link_libraries(TestRadialLayout
    rocsgraphtheory
    Qt5::Test
)
target_compile_definitions(TestRadialLayout PRIVATE ADDITIONAL_PLUGIN_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")


set(qualitybenchmark_SRCS
    qualitybenchmark.cpp
    metricsummarizer.cpp
    layoutevaluator.cpp
)

add_executable(QualityBenchmark ${qualitybenchmark_SRCS})
target_link_libraries(QualityBenchmark
    rocsgraphtheory
)
target_compile_definitions(QualityBenchmark PRIVATE ADDITIONAL_PLUGIN_DIR="${CMAKE_BINARY_DIR}/bin/plugins")
