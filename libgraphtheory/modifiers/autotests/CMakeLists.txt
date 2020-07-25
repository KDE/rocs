#  Copyright 2020  Dilson Almeida Guimarães <dilsonguim@gmail.com>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) version 3, or any
#  later version accepted by the membership of KDE e.V. (or its
#  successor approved by the membership of KDE e.V.), which shall
#  act as a proxy defined in Section 6 of version 3 of the license.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library.  If not, see <https://www.gnu.org/licenses/>.

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

set(testradiallayout_SRCS
    testradiallayout.cpp
)


add_executable(TestRadialLayout ${testradiallayout_SRCS})
add_test(NAME TestRadialLayout COMMAND TesRadialLayout)
ecm_mark_as_test(TestRadialLayout)
target_link_libraries(TestRadialLayout
    rocsgraphtheory
    Qt5::Test
)


set(qualitybenchmark_SRCS
    qualitybenchmark.cpp
    metricsummarizer.cpp
    layoutevaluator.cpp
)

add_executable(QualityBenchmark ${qualitybenchmark_SRCS})
target_link_libraries(QualityBenchmark
    rocsgraphtheory
)
