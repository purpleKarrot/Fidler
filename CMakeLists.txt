# Copyright (c) 2014, Daniel Pfeifer <daniel@pfeifer-mail.de>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Fidler VERSION 0.1 LANGUAGES CXX)

enable_testing()

add_library(libfidler
  src/franca/comment_grammar.cpp
  src/franca/comment_grammar.hpp
  src/franca/contract_grammar.cpp
  src/franca/contract_grammar.hpp
  src/franca/expression_grammar.cpp
  src/franca/expression_grammar.hpp
  src/franca/initializer_grammar.cpp
  src/franca/initializer_grammar.hpp
  src/franca/typesystem_grammar.cpp
  src/franca/typesystem_grammar.hpp
  src/franca/model_grammar.cpp
  src/franca/model_grammar.hpp
  src/cxx/expression.cpp
  src/cxx/expression.hpp
  src/cxx/initializer.cpp
  src/cxx/initializer.hpp
  src/cxx/model.cpp
  src/cxx/model.hpp
  src/cxx/typesystem.cpp
  src/cxx/typesystem.hpp
  src/util/algorithm.cpp
  src/cxx.cpp
  src/franca.cpp
  )
set_target_properties(libfidler PROPERTIES OUTPUT_NAME fidler)
target_include_directories(libfidler PUBLIC include)
target_compile_options(libfidler PUBLIC -std=c++11)

add_executable(fidler
  src/main.cpp
  )
target_link_libraries(fidler PRIVATE libfidler)

file(GLOB_RECURSE fidl_files "*.fidl")
foreach(file IN LISTS fidl_files)
  get_filename_component(name "${file}" NAME_WE)
  get_filename_component(directory "${file}" DIRECTORY)
  file(RELATIVE_PATH directory "${Fidler_SOURCE_DIR}" "${directory}")
  string(REGEX REPLACE "[./]" ";" name "${directory}.${name}")
  list(REMOVE_ITEM name org franca fidl testcases tools tool model models)
  list(REMOVE_DUPLICATES name)
  string(REPLACE ";" "." name "${name}")
  add_test(NAME "${name}"
    COMMAND $<TARGET_FILE:fidler> ${file}
    )
endforeach()
