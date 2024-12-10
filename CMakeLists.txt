cmake_minimum_required(VERSION 3.14)
project(example)
set(CMAKE_CXX_STANDARD 17) 

#find_package(aare REQUIRED)

add_subdirectory(aare)
add_subdirectory(tiffio)

#include_directories(${CMAKE_BINARY_DIR}/tiffio/include/sls)
#List of examples to be compiled
set(ExampleSrc
    00-NDArray.cpp
)

#Iterate over the list of examples and compile them
foreach(FILE ${ExampleSrc})
    get_filename_component(EXENAME ${FILE} NAME_WE)
    add_executable(${EXENAME} ${FILE})
    target_link_libraries(${EXENAME} tiffio aare_core)
    set_target_properties(${EXENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endforeach()

#option(ENABLE_ROOT_EXAMPLES "Enable ROOT examples" ON)



#if(ENABLE_ROOT_EXAMPLES)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT CONFIG REQUIRED)

    set(RootExampleSrc
        04-TH1.cpp
    )

    foreach(FILE ${RootExampleSrc})
        get_filename_component(EXENAME ${FILE} NAME_WE)
        add_executable(${EXENAME} ${FILE})
        target_link_libraries(${EXENAME} tiffio aare_core ROOT::Hist ROOT::Graf ROOT::Gpad ROOT::Core)
        set_target_properties(${EXENAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    endforeach()
    #endif()
