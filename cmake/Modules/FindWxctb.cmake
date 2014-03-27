# - Try to find wxctb-0.13
# Once done, this will define
#
#  WXCTB_FOUND - system has wxctb
#  WXCTB_INCLUDE_DIR - the wxctb include directories
#  WXCTB_LIBRARY - link these to use wxctb

find_path(WXCTB_INCLUDE_DIR
	ctb.h
	/usr/local/include/wx-3.0/wx/ctb-0.13
)

find_library(WXCTB_LIBRARY
	NAMES libwxctb-0.13.so
	PATHS
	/usr/local/lib
)

message( STATUS "WXCTB INCLUDE DIR " ${WXCTB_INCLUDE_DIR})
message( STATUS "WXCTB LIBRARY " ${WXCTB_LIBRARY})

if(WXCTB_INCLUDE_DIR AND WXCTB_LIBRARY)
  set(WXCTB_FOUND TRUE)
  message (STATUS "WXCTB project found " ${WXCTB_LIBRARIES})
else (WXCTB_INCLUDE_DIR AND WXCTB_LIBRARY)
  set(WXCTB_FOUND FALSE)
  if(WXCTB_FIND_REQUIRED)
    message( FATAL_ERROR "WXCTB project was not found!")
  else (WXCTB_FIND_REQUIRED)
    if(NOT WXCTB_FIND_QUIETLY)
      message( STATUS "WXCTB project was not found!")
    endif(NOT WXCTB_FIND_QUIETLY)
  endif(WXCTB_FIND_REQUIRED)
endif(WXCTB_INCLUDE_DIR AND WXCTB_LIBRARY)

mark_as_advanced(
		WXCTB_FOUND
		WXCTB_INCLUDE_DIR
		WXCTB_LIBRARY
)
