# - Try to find libconfig
# Once done, this will define
#
#  LIBPUGIXML_FOUND - system has libconfig
#  LIBPUGIXML_INCLUDE_DIR - the libconfig include directories
#  LIBPUGIXML_LIBRARY - link these to use libconfig

find_path(LIBPUGIXML_INCLUDE_DIR
  pugixml.hpp
	/home/stretcher/stretcher/include
	/usr/local/include
)

find_library(LIBPUGIXML_LIBRARY
  NAMES libpugixml.so
	PATHS
	/home/stretcher/stretcher/lib
	/usr/local/lib
)

message( STATUS "LIBPUGIXML INCLUDE DIR " ${LIBPUGIXML_INCLUDE_DIR})
message( STATUS "LIBPUGIXML LIBRARY " ${LIBPUGIXML_LIBRARY})

if(LIBPUGIXML_INCLUDE_DIR AND LIBPUGIXML_LIBRARY)
  set(LIBPUGIXML_FOUND TRUE)
  message( STATUS "LIBPUGIXML project found " ${LIBPUGIXML_LIBRARIES})
else(LIBPUGIXML_INCLUDE_DIR AND LIBPUGIXML_LIBRARY)
  set(LIBPUGIXML_FOUND FALSE)
  if(LIBPUGIXML_FIND_REQUIRED)
    message( FATAL_ERROR "LIBPUGIXML project was not found!")
  else(LIBPUGIXML_FIND_REQUIRED)
   if(NOT LIBPUGIXML_FIND_QUIETLY)
     message( STATUS "LIBPUGIXML project was not found!")
   endif(NOT LIBPUGIXML_FIND_QUIETLY)
  endif(LIBPUGIXML_FIND_REQUIRED)
endif(LIBPUGIXML_INCLUDE_DIR AND LIBPUGIXML_LIBRARY)

mark_as_advanced(
    LIBPUGIXML_FOUND
    LIBPUGIXML_INCLUDE_DIR
    LIBPUGIXML_LIBRARY
)
