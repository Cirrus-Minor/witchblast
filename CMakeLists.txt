cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)

# Projet name
project("Witch_Blast")

# Detect compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(USING_GCC TRUE)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(USING_CLANG TRUE)
endif()

# For GCC and Clang, enable C++11 support and add some other flags
if(USING_GCC OR USING_CLANG)
  add_compile_options(-std=c++11 -pedantic -Wall)
endif()

# Compilation options
option(ONLINE_MODE "Enable online mode, which requires src/OnlineScoring.h" OFF)

if(ONLINE_MODE)
  add_compile_options(-DONLINE_MODE)
endif()

include_directories(.)

file(
        GLOB_RECURSE
        source_files
        src/*
)

if(APPLE)
  set(${source_files} “${source_files} src/ressources/witchblast.icns”)
  set_source_files_properties(src/ressources/witchblast.icns
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  set(MACOSX_BUNDLE_ICON_FILE witchblast.icns)
endif()

add_executable(
        "Witch_Blast"
        MACOSX_BUNDLE
        ${source_files}
)
set_target_properties(Witch_Blast PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
)
if(APPLE)
	set_target_properties(Witch_Blast PROPERTIES
		MACOSX_RPATH 1
		BUILD_WITH_INSTALL_RPATH 1
		INSTALL_RPATH "@loader_path/../Frameworks")
  set(EXTRA_LIBRARIES "-framework CoreFoundation")
endif()

if(UNIX)
	find_package(Threads REQUIRED)
	if(THREADS_HAVE_PTHREAD_ARG)
	  target_compile_options(PUBLIC Witch_Blast "-pthread")
	endif()
	if(CMAKE_THREAD_LIBS_INIT)
	  target_link_libraries(Witch_Blast "${CMAKE_THREAD_LIBS_INIT}")
	endif()
endif()

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
if(ONLINE_MODE)
  find_package(SFML 2.2 REQUIRED system window graphics audio network)
else()
  find_package(SFML 2.2 REQUIRED system window graphics audio)
endif()
target_link_libraries(Witch_Blast ${SFML_LIBRARIES} ${EXTRA_LIBRARIES})

include_directories(${SFML_INCLUDE_DIR})

Message(${SFML_LIBRARIES})

if(APPLE)
	install(
		DIRECTORY Witch_Blast.app
		DESTINATION "."
		USE_SOURCE_PERMISSIONS)
  install(
    DIRECTORY
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_SOURCE_DIR}/media
    DESTINATION Witch_Blast.app/Contents/Resources)
  # copy SFML frameworks into app bundle for Mac OS X
  foreach(LIB ${SFML_LIBRARIES})
    install(DIRECTORY ${LIB}
      DESTINATION Witch_Blast.app/Contents/Frameworks)
  endforeach()
  set(SFML_LIBRARIES_EXTRA
    SFML FLAC freetype ogg OpenAL vorbis vorbisenc vorbisfile)
  foreach(LIB ${SFML_LIBRARIES_EXTRA})
    install(DIRECTORY /Library/Frameworks/${LIB}.framework
      DESTINATION Witch_Blast.app/Contents/Frameworks)
  endforeach()
  install(FILES
    ${CMAKE_SOURCE_DIR}/COPYING.txt
    ${CMAKE_SOURCE_DIR}/readme.txt
    DESTINATION ".")
endif()

# Packaging
SET(CPACK_PACKAGE_VERSION "0.7.5")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "7")
SET(CPACK_PACKAGE_VERSION_PATCH "5")
SET(CPACK_PACKAGE_EXECUTABLES "Witch_Blast;Witch Blast")
if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
  set(CPACK_DMG_FORMAT "UDBZ")
  set(CPACK_DMG_VOLUME_NAME "Witch Blast")
  set(CPACK_SYSTEM_NAME "OSX")
  set(CPACK_PACKAGE_ICON src/ressources/witchblast.icns)
  # TODO: CPACK_DMG_BACKGROUND_IMAGE
  # TODO: CPACK_DMG_DS_STORE
endif()
include(CPack)
