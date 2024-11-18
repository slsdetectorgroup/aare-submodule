cmake_minimum_required(VERSION 3.14)
project(example)
set(CMAKE_CXX_STANDARD 17) 


add_subdirectory(aare)


#List of examples to be compiled
set(ExampleSrc
    00-NDArray.cpp
)

#Iterate over the list of examples and compile them
foreach(FILE ${ExampleSrc})
    get_filename_component(EXENAME ${FILE} NAME_WE)
    add_executable(${EXENAME} ${FILE})
    target_link_libraries(${EXENAME}  aare_core)
    set_target_properties(${EXENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()
