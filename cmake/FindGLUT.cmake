#.rst:
# FindGLUT
# --------
#
# try to find glut library and include files.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` targets:
#
# ``GLUT::GLUT``
#  Defined if the system has GLUT.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ::
#
#   GLUT_INCLUDE_DIR, where to find GL/glut.h, etc.
#   GLUT_LIBRARIES, the libraries to link against
#   GLUT_FOUND, If false, do not try to use GLUT.
#
# Also defined, but not for general use are:
#
# ::
#
#   GLUT_glut_LIBRARY = the full path to the glut library.
#   GLUT_Xmu_LIBRARY  = the full path to the Xmu library.
#   GLUT_Xi_LIBRARY   = the full path to the Xi Library.


#=============================================================================
# CMake - Cross Platform Makefile Generator
# Copyright 2000-2015 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# ------------------------------------------------------------------------------
# 
# The above copyright and license notice applies to distributions of
# CMake in source and binary form.  Some source files contain additional
# notices of original copyright by their contributors; see each source
# for details.  Third-party software packages supplied with CMake under
# compatible licenses provide their own copyright notices documented in
# corresponding subdirectories.
# 
# ------------------------------------------------------------------------------
# 
# CMake was initially developed by Kitware with the following sponsorship:
# 
#  * National Library of Medicine at the National Institutes of Health
#    as part of the Insight Segmentation and Registration Toolkit (ITK).
# 
#  * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel
#    Visualization Initiative.
# 
#  * National Alliance for Medical Image Computing (NAMIC) is funded by the
#    National Institutes of Health through the NIH Roadmap for Medical Research,
#    Grant U54 EB005149.
# 
#  * Kitware, Inc.
#=============================================================================

if (WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
  else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
  endif()
  find_path( GLUT_INCLUDE_DIR NAMES GL/glut.h
    PATHS  $ENV{GLUT_ROOT_PATH}/include )
  find_library( GLUT_glut_LIBRARY NAMES glut glut32 freeglut
    PATHS ${OPENGL_LIBRARY_DIR} $ENV{GLUT_ROOT_PATH}
	PATH_SUFFIXES ${VC_LIB_PATH_SUFFIX} lib
    )
else ()

  if (APPLE)
    find_path(GLUT_INCLUDE_DIR glut.h ${OPENGL_LIBRARY_DIR})
    find_library(GLUT_glut_LIBRARY GLUT DOC "GLUT library for OSX")
    find_library(GLUT_cocoa_LIBRARY Cocoa DOC "Cocoa framework for OSX")

    if(GLUT_cocoa_LIBRARY AND NOT TARGET GLUT::Cocoa)
      add_library(GLUT::Cocoa UNKNOWN IMPORTED)
      # Cocoa should always be a Framework, but we check to make sure.
      if(GLUT_cocoa_LIBRARY MATCHES "/([^/]+)\\.framework$")
        set_target_properties(GLUT::Cocoa PROPERTIES
          IMPORTED_LOCATION "${GLUT_cocoa_LIBRARY}/${CMAKE_MATCH_1}")
      else()
        set_target_properties(GLUT::Cocoa PROPERTIES
          IMPORTED_LOCATION "${GLUT_cocoa_LIBRARY}")
      endif()
    endif()
  else ()

    if (BEOS)

      set(_GLUT_INC_DIR /boot/develop/headers/os/opengl)
      set(_GLUT_glut_LIB_DIR /boot/develop/lib/x86)

    else()

      find_library( GLUT_Xi_LIBRARY Xi
        /usr/openwin/lib
        )

      find_library( GLUT_Xmu_LIBRARY Xmu
        /usr/openwin/lib
        )

      if(GLUT_Xi_LIBRARY AND NOT TARGET GLUT::Xi)
        add_library(GLUT::Xi UNKNOWN IMPORTED)
        set_target_properties(GLUT::Xi PROPERTIES
          IMPORTED_LOCATION "${GLUT_Xi_LIBRARY}")
      endif()

      if(GLUT_Xmu_LIBRARY AND NOT TARGET GLUT::Xmu)
        add_library(GLUT::Xmu UNKNOWN IMPORTED)
        set_target_properties(GLUT::Xmu PROPERTIES
          IMPORTED_LOCATION "${GLUT_Xmu_LIBRARY}")
      endif()

    endif ()

    find_path( GLUT_INCLUDE_DIR GL/glut.h
      /usr/include/GL
      /usr/openwin/share/include
      /usr/openwin/include
      /opt/graphics/OpenGL/include
      /opt/graphics/OpenGL/contrib/libglut
      ${_GLUT_INC_DIR}
      )

    find_library( GLUT_glut_LIBRARY glut
      /usr/openwin/lib
      ${_GLUT_glut_LIB_DIR}
      )

    unset(_GLUT_INC_DIR)
    unset(_GLUT_glut_LIB_DIR)

  endif ()

endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLUT REQUIRED_VARS GLUT_glut_LIBRARY GLUT_INCLUDE_DIR)

if (GLUT_FOUND)
  # Is -lXi and -lXmu required on all platforms that have it?
  # If not, we need some way to figure out what platform we are on.
  set( GLUT_LIBRARIES
    ${GLUT_glut_LIBRARY}
    ${GLUT_Xmu_LIBRARY}
    ${GLUT_Xi_LIBRARY}
    ${GLUT_cocoa_LIBRARY}
    )

  if(NOT TARGET GLUT::GLUT)
    add_library(GLUT::GLUT UNKNOWN IMPORTED)
    set_target_properties(GLUT::GLUT PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${GLUT_INCLUDE_DIR}")
    if(GLUT_glut_LIBRARY MATCHES "/([^/]+)\\.framework$")
      set_target_properties(GLUT::GLUT PROPERTIES
        IMPORTED_LOCATION "${GLUT_glut_LIBRARY}/${CMAKE_MATCH_1}")
    else()
      set_target_properties(GLUT::GLUT PROPERTIES
        IMPORTED_LOCATION "${GLUT_glut_LIBRARY}")
    endif()

    if(TARGET GLUT::Xmu)
      set_property(TARGET GLUT::GLUT APPEND
        PROPERTY INTERFACE_LINK_LIBRARIES GLUT::Xmu)
    endif()

    if(TARGET GLUT::Xi)
      set_property(TARGET GLUT::GLUT APPEND
        PROPERTY INTERFACE_LINK_LIBRARIES GLUT::Xi)
    endif()

    if(TARGET GLUT::Cocoa)
      set_property(TARGET GLUT::GLUT APPEND
        PROPERTY INTERFACE_LINK_LIBRARIES GLUT::Cocoa)
    endif()
  endif()

  #The following deprecated settings are for backwards compatibility with CMake1.4
  set (GLUT_LIBRARY ${GLUT_LIBRARIES})
  set (GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})
endif()

mark_as_advanced(
  GLUT_INCLUDE_DIR
  GLUT_glut_LIBRARY
  GLUT_Xmu_LIBRARY
  GLUT_Xi_LIBRARY
  )
