# Install script for directory: E:/lib/leptonica/leptonica-1.74.4/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/leptonica")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/Debug/leptonica-1.74.4d.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/Release/leptonica-1.74.4.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/MinSizeRel/leptonica-1.74.4.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/RelWithDebInfo/leptonica-1.74.4.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/lib/leptonica/leptonica-1.74.4/build/bin/Debug/leptonica-1.74.4d.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/lib/leptonica/leptonica-1.74.4/build/bin/Release/leptonica-1.74.4.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/lib/leptonica/leptonica-1.74.4/build/bin/MinSizeRel/leptonica-1.74.4.dll")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "E:/lib/leptonica/leptonica-1.74.4/build/bin/RelWithDebInfo/leptonica-1.74.4.dll")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/LeptonicaTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/LeptonicaTargets.cmake"
         "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/LeptonicaTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/cmake/LeptonicaTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake" TYPE FILE FILES "E:/lib/leptonica/leptonica-1.74.4/build/src/CMakeFiles/Export/cmake/LeptonicaTargets-release.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/leptonica" TYPE FILE FILES
    "E:/lib/leptonica/leptonica-1.74.4/src/allheaders.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/alltypes.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/array.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/arrayaccess.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/bbuffer.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/bilateral.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/bmf.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/bmfdata.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/bmp.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/ccbord.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/dewarp.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/environ.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/gplot.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/heap.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/imageio.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/jbclass.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/leptwin.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/list.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/morph.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/pix.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/ptra.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/queue.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/rbtree.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/readbarcode.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/recog.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/regutils.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/stack.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/stringcode.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/sudoku.h"
    "E:/lib/leptonica/leptonica-1.74.4/src/watershed.h"
    "E:/lib/leptonica/leptonica-1.74.4/build/src/endianness.h"
    )
endif()

