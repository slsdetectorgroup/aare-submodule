find_package(TIFF REQUIRED)
set(PUBLICHEADERS
    include/sls/tiffIO.h
)
set(SourceFiles
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tiffIO.cpp
)
add_library(tiffio STATIC src/tiffIO.cpp)
target_include_directories(tiffio PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
   "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_link_libraries(tiffio 
    PUBLIC 
        TIFF::TIFF 
)



set_target_properties(tiffio PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    PUBLIC_HEADER "${PUBLICHEADERS}"
)

