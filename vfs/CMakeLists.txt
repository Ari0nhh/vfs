cmake_minimum_required (VERSION 3.8)

project ("vfs" VERSION 1.0 DESCRIPTION "Simple virtual filesystem library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library (vfs SHARED 
	"include/vfs.h"
	"src/volume/volume.h"
	"src/volume/volume.cpp"
)

if(MSVC)
  target_compile_options(vfs PRIVATE /W4 /WX)
else()
  target_compile_options(vfs PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

set_target_properties(vfs PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vfs PROPERTIES PUBLIC_HEADER "include/vfs.h")

install(TARGETS vfs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})