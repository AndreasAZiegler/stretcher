# - Try to find libconfig
# Once done, this will define
#
#  LIBCONFIG_FOUND - system has libconfig
#  LIBCONFIG_INCLUDE_DIR - the libconfig include directories
#  LIBCONFIG_LIBRARY - link these to use libconfig

find_path(LIBCONFIG_INCLUDE_DIR
	libconfig.h++
	/home/stretcher/stretcher/include
	/usr/local/include
)

find_library(LIBCONFIG_LIBRARY
	NAMES libconfig++.so
	PATHS
	/home/stretcher/stretcher/lib
	/usr/local/lib
)

message( STATUS "LIBCONFIG INCLUDE DIR " ${LIBCONFIG_INCLUDE_DIR})
message( STATUS "LIBCONFIG LIBRARY " ${LIBCONFIG_LIBRARY})

if(LIBCONFIG_INCLUDE_DIR AND LIBCONFIG_LIBRARY)
  set(LIBCONFIG_FOUND TRUE)
  message( STATUS "LIBCONFIG project found " ${LIBCONFIG_LIBRARIES})
else(LIBCONFIG_INCLUDE_DIR AND LIBCONFIG_LIBRARY)
  set(LIBCONFIG_FOUND FALSE)
  if(LIBCONFIG_FIND_REQUIRED)
    message( FATAL_ERROR "LIBCONFIG project was not found!")
  else(LIBCONFIG_FIND_REQUIRED)
   if(NOT LIBCONFIG_FIND_QUIETLY)
     message( STATUS "LIBCONFIG project was not found!")
   endif(NOT LIBCONFIG_FIND_QUIETLY)
  endif(LIBCONFIG_FIND_REQUIRED)
endif(LIBCONFIG_INCLUDE_DIR AND LIBCONFIG_LIBRARY)

mark_as_advanced(
		LIBCONFIG_FOUND
		LIBCONFIG_INCLUDE_DIR
		LIBCONFIG_LIBRARY
)
