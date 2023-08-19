add_library(Roulette roulette.cpp)

target_include_directories(Roulette 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
    )

target_link_libraries(Roulette oo_design_compiler_flags)