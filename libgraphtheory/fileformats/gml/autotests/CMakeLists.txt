# Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

file(COPY example.gml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(testgmlfileformat_SRCS
    testgmlfileformat.cpp
    ../gmlgrammar.cpp
    ../gmlgrammarhelper.cpp
    ../gmlfileformat.cpp
    ../../../logging.cpp
)

add_executable(TestGmlFileFormat ${testgmlfileformat_SRCS})
add_test(TestGmlFileFormat TestGmlFileFormat)
ecm_mark_as_test(TestGmlFileFormat)
target_link_libraries(TestGmlFileFormat
    rocsgraphtheory
    Qt5::Test
)

# Boost::spirit requires exceptions
kde_target_enable_exceptions(TestGmlFileFormat PRIVATE)
