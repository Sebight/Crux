# CMakeList.txt : CMake project for Crux, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Crux")

# Add source to this project's executable.
add_executable (Crux "Crux.cpp" "Crux.h" "Scanner.cpp" "Scanner.h" "Token.h" "Literal.h" "Literal.cpp" "AST.h"  "Visitor.h" "AST.cpp" "ASTPrinter.h" "ASTPrinter.cpp" "Parser.h" "Parser.cpp" "Types.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Crux PROPERTY CXX_STANDARD 20)
  # Output name to "crxx"
  set_target_properties(Crux PROPERTIES OUTPUT_NAME "crux")
endif()

# TODO: Add tests and install targets if needed.
