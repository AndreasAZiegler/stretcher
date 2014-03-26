# - Try to find libmathplot
# Once done, this will define
#
#  WXMATHPLOT_FOUND - system has wxmathplot
#  WXMATHPLOT_INCLUDE_DIR - the wxmathplot include directories
#  WXMATHPLOT_LIBRARY - link these to use wxmathplot

find_path(WXMATHPLOT_INCLUDE_DIR
	mathplot.h
	/usr/local/include
)

find_library(WXMATHPLOT_LIBRARY
	NAMES libmathplot.so
	PATHS
	/usr/local/lib64
)

message( STATUS "WXMATHPLOT INCLUDE DIR " ${WXMATHPLOT_INCLUDE_DIR})
message( STATUS "WXMATHPLOT LIBRARY " ${WXMATHPLOT_LIBRARY})

if(WXMATHPLOT_INCLUDE_DIR AND WXMATHPLOT_LIBRARY)
  set(WXMATHPLOT_FOUND TRUE)
  message(STATUS "WXMATHPLOT project found " ${WXMATHPLOT_LIBRARIES})
else(WXMATHPLOT_INCLUDE_DIR AND WXMATHPLOT_LIBRARY)
  set(WXMATHPLOT_FOUND FALSE)
  if(WXMATHPLOT_FIND_REQUIRED)
    message( FATAL_ERROR "WXMATHPLOT project was not found!")
  else(WXMATHPLOT_FIND_REQUIRED)
    if(NOT WXMATHPLOT_FIND_QUIETLY)
      message( STATUS "WXMATHPLOT project was not found!")
    endif(NOT WXMATHPLOT_FIND_QUIETLY)
  endif(WXMATHPLOT_FIND_REQUIRED)
endif(WXMATHPLOT_INCLUDE_DIR AND WXMATHPLOT_LIBRARY)

mark_as_advanced(
		WXMATHPLOT_FOUND
		WXMATHPLOT_INCLUDE_DIR
		WXCTB_LIBRARY
)
