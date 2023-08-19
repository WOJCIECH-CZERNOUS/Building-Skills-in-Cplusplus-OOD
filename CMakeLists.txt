# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)

# set the project name and version
project(oo_design VERSION 0.1.0)

add_library(oo_design_compiler_flags INTERFACE)
target_compile_features(oo_design_compiler_flags INTERFACE cxx_std_17)

# add compiler warning flags just when building this project via
# the BUILD_INTERFACE genex
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(oo_design_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

if(APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
elseif(UNIX)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

# add libraries
add_subdirectory(Roulette)
list(APPEND EXTRA_LIBS Roulette)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/Roulette")

# add the executable
add_executable(OODesign main.cpp)

target_link_libraries(OODesign PUBLIC ${EXTRA_LIBS})
target_link_libraries(OODesign PUBLIC oo_design_compiler_flags)

# add to the search path, so that we will find .hpp
target_include_directories(OODesign PUBLIC
  "${PROJECT_BINARY_DIR}"
  ${EXTRA_INCLUDES}
  )   

# add the install targets
install(TARGETS OODesign DESTINATION bin)

# enable testing
enable_testing()

# use a loop to simplify adding tests
foreach(arg RANGE 1 11)
  add_test(NAME test${arg} COMMAND OODesign ${arg})
  set_tests_properties(test${arg} PROPERTIES PASS_REGULAR_EXPRESSION "OK")
endforeach()

set_tests_properties(test8 PROPERTIES PASS_REGULAR_EXPRESSION "Passenger57 lost 10")
set_tests_properties(test9 PROPERTIES PASS_REGULAR_EXPRESSION "Passenger57 won 20")
set_tests_properties(test10 PROPERTIES PASS_REGULAR_EXPRESSION "Bets 10, 90 left. Lost. Bets 20, 70 left. Won 40 and now has 110. Bets 10, 100 left. Lost. Bets 20, 80 left. Lost. Bets 40, 40 left. Lost. Wants to bet 80, but has only 40. No more bets. No more bets.")

