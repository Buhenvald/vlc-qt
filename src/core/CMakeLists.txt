#############################################################################
# VLC-Qt - Qt and libvlc connector library
# Copyright (C) 2013 Tadej Novak <tadej@tano.si>
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.
#############################################################################
# Configuration #
#################

# Add definition for exporting symbols
ADD_DEFINITIONS(-DVLCQT_CORE_LIBRARY)

# Add definitions for shared or static library
IF(STATIC)
    ADD_DEFINITIONS(-DQT_STATIC -D_REENTRANT)
ELSE(STATIC)
    ADD_DEFINITIONS(-DQT_SHARED -D_REENTRANT)
ENDIF(STATIC)

IF(MINGW OR MSVC)
    CONFIGURE_FILE(
        ${CMAKE_SOURCE_DIR}/src/platform/windows/core.rc.in
        ${CMAKE_BINARY_DIR}/src/platform/windows/core.rc
    )
ENDIF(MINGW OR MSVC)

###################
# List of sources #
###################
# Define the C++ source files
SET(VLCQT_CORE_SRCS
    Audio.cpp
    Common.cpp
    Enums.cpp
    Error.cpp
    Instance.cpp
    Media.cpp
    MediaList.cpp
    MediaListPlayer.cpp
    MediaPlayer.cpp
    MetaManager.cpp
    SharedExportCore.h
    Video.cpp
    VideoDelegate.h
    VideoFrame.cpp
)

IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(VLCQT_CORE_SRCS
        ${VLCQT_CORE_SRCS}
        VideoMemoryStream.cpp
    )
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

# Define the Include files
SET(VLCQT_CORE_HEADERS
    Audio.h
    Common.h
    Enums.h
    Error.h
    Instance.h
    Media.h
    MediaList.h
    MediaListPlayer.h
    MediaPlayer.h
    MetaManager.h
    SharedExportCore.h
    Video.h
    VideoDelegate.h
    VideoFrame.h
    VideoMemoryStream.h

    ${CMAKE_BINARY_DIR}/src/Config.h
)

# Define the resource files
SET(VLCQT_CORE_RESOURCES
    ../../data/logo.qrc
)


#####################
# Compiler settings #
#####################
# This scripts run the resource compiler rcc
IF(QT5)
    QT5_ADD_RESOURCES(VLCQT_CORE_RESOURCES_SRCS ${VLCQT_CORE_RESOURCES})
ELSE(QT5)
    QT4_ADD_RESOURCES(VLCQT_CORE_RESOURCES_SRCS ${VLCQT_CORE_RESOURCES})
ENDIF(QT5)

INCLUDE_DIRECTORIES(
    ${LIBVLC_INCLUDE_DIR}
    ${LIBVLC_INCLUDE_DIR}/vlc/plugins
)

INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_BINARY_DIR}/src
)

IF(MSVC)
    INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/windows/msvc)
ENDIF(MSVC)

# Compile resources with windres
IF(MINGW)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${CMAKE_BINARY_DIR}/src/platform/windows/core.obj
        COMMAND ${CMAKE_RC_COMPILER}
        -i ${CMAKE_BINARY_DIR}/src/platform/windows/core.rc
        -o ${CMAKE_BINARY_DIR}/src/platform/windows/core.obj
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/src/platform/windows"
    )
    SET(VLCQT_CORE_SRCS ${VLCQT_CORE_SRCS} ${CMAKE_BINARY_DIR}/src/platform/windows/core.obj)
ENDIF(MINGW)

IF(MSVC)
    SET(VLCQT_CORE_SRCS ${VLCQT_CORE_SRCS} ${CMAKE_BINARY_DIR}/src/platform/windows/core.rc)
ENDIF(MSVC)

# Create the library.
IF(STATIC)
    ADD_LIBRARY(${VLCQT} STATIC
                ${VLCQT_CORE_SRCS} ${VLCQT_CORE_RESOURCES_SRCS})
ELSE(STATIC)
    ADD_LIBRARY(${VLCQT} SHARED
                ${VLCQT_CORE_SRCS} ${VLCQT_CORE_RESOURCES_SRCS})
ENDIF(STATIC)

# MSVC: Avoid Debug/Release/... subfolders
IF(MSVC)
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ".")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ".")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ".")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDUBUGINFO ".")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ".")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_PROFILE ".")
ENDIF(MSVC)

# Linux: So version extensions
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET_TARGET_PROPERTIES(${VLCQT} PROPERTIES VERSION ${VLCQT_VERSION}
                                          SOVERSION "${VLCQT_VERSION_MAJOR}.${VLCQT_VERSION_MINOR}"
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Link the required libraries
IF(QT5)
    QT5_USE_MODULES(${VLCQT} Core)
    TARGET_LINK_LIBRARIES(${VLCQT} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
ELSE(QT5)
    TARGET_LINK_LIBRARIES(${VLCQT} ${QT_LIBRARIES} ${LIBVLC_LIBRARY} ${LIBVLCCORE_LIBRARY})
ENDIF(QT5)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)
    ADD_CUSTOM_COMMAND(TARGET ${VLCQT} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "@loader_path/lib/libvlc.5.dylib" "@loader_path/../lib/libvlc.5.dylib" $<TARGET_FILE:${VLCQT}>)
    ADD_CUSTOM_COMMAND(TARGET ${VLCQT} POST_BUILD COMMAND
        ${CMAKE_INSTALL_NAME_TOOL} -change "@loader_path/lib/libvlccore.5.dylib" "@loader_path/../lib/libvlccore.5.dylib" $<TARGET_FILE:${VLCQT}>)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT STATIC)


################
# Installation #
################
INSTALL(TARGETS ${VLCQT} RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX}
)
INSTALL(FILES ${VLCQT_CORE_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/vlc-qt)
