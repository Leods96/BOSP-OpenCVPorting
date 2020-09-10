###############################################################################
# The AEM Delaunay2 Application
################################################################################

cmake_minimum_required(VERSION 3.0)

#----- Check for the required RTLib library
find_package(BbqRTLib REQUIRED)
find_package( OpenCV REQUIRED )

#----- Add compilation dependencies
include_directories(${BBQUE_RTLIB_INCLUDE_DIR}
					${OpenCV_INCLUDE_DIRS} )

#----- Add "delaunay2" target application
set(DELAUNAY2_SRC version Delaunay2_exc Delaunay2_main)
add_executable(delaunay2 ${DELAUNAY2_SRC})

#----- Linking dependencies
target_link_libraries(
	delaunay2
	${Boost_LIBRARIES}
	${BBQUE_RTLIB_LIBRARY}
	${OpenCV_LIBS} 
)

# Use link path ad RPATH
set_property(TARGET delaunay2 PROPERTY
	INSTALL_RPATH_USE_LINK_PATH TRUE)

#----- Install the Delaunay2 files
install (TARGETS delaunay2 RUNTIME
	DESTINATION ${DELAUNAY2_PATH_BINS})

#----- Generate and Install Delaunay2 configuration file
configure_file (
	"${PROJECT_SOURCE_DIR}/Delaunay2.conf.in"
	"${PROJECT_BINARY_DIR}/Delaunay2.conf"
)
install (FILES "${PROJECT_BINARY_DIR}/Delaunay2.conf"
	DESTINATION ${DELAUNAY2_PATH_CONFIG})
