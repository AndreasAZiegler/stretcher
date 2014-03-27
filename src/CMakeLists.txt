##---------------------------------------------------------------------------
## Author:      Andreas Ziegler
##---------------------------------------------------------------------------

# Add the subdirectories to the CMake configuration
include_directories("./gui")
include_directories("./hardware")

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 
 
# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# http://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html
# We need the Find package for wxWidgets to work
# NOTE: if you're using aui, include aui in this required components list.
 
# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base xrc REQUIRED)
 
##---------------------------------------------------
## Actual config file starts here
##--------------------------------------------------- 
 
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )
 
# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
set(SOURCE main.cpp settings.cpp)
set(HEADERS main.h settings.h)

# Add gui part
add_subdirectory(./gui)
# Add hardware parts
add_subdirectory(./hardware)

# Add library for the serial interface wxctb-0.13
find_package(Wxctb REQUIRED)
if(WXCTB_FOUND)
  set(EXTERN_INCLUDES ${EXTERN_INCLUDES} ${WXCTB_INCLUDE_DIR})
  set(EXTERN_LIBS ${EXTERN_LIBS} ${WXCTB_LIBRARY})
endif(WXCTB_FOUND)

# Add library for the graph wxmathplot
find_package(Libmathplot REQUIRED)
if(WXMATHPLOT_FOUND)
  set(EXTERN_INCLUDES ${EXTERN_INCLUDES} ${WXMATHPLOT_INCLUDE_DIR})
  set(EXTERN_LIBS ${EXTERN_LIBS} ${WXMATHPLOT_LIBRARY})
endif(WXMATHPLOT_FOUND)

# Add library for loading and saving of settings
find_package(Libconfig REQUIRED)
if(LIBCONFIG_FOUND)
  set(EXTERN_INCLUDES ${EXTERN_INCLUDES} ${LIBCONFIG_INCLUDE_DIR})
  set(EXTERN_LIBS ${EXTERN_LIBS} ${LIBCONFIG_LIBRARY})
endif(LIBCONFIG_FOUND)


#set_target_properties(libgpPanel PROPERTIES LINKER_LANGUAGE C)
#install(TARGETS libwxctb DESTINATION ../build)
 
# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(SOURCE ${SOURCE} stretcher.rc)
endif(WIN32)
 
# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_executable(Stretcher ${SOURCE} ${HEADERS})
 
# We add to our target 'minimal' the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well. 
include_directories(${EXTERN_INCLUDES})
target_link_libraries(Stretcher ${wxWidgets_LIBRARIES} "${EXTERN_LIBS}")

# Installation
install(TARGETS Stretcher DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/src/gui" DESTINATION bin/gui)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/lib" DESTINATION bin)
