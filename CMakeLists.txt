CMAKE_MINIMUM_REQUIRED (VERSION 2.4.7)

PROJECT (FILE_BROWSER_APPLET C CXX)

# Compiler flags
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall -ansi")
SET (EXEC file-browser-applet)

# Version
################################################################################
SET (VERSION_MAJOR 0)
SET (VERSION_MINOR 6)
SET (VERSION_PATCH 8)

# Install path options
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_PREFIX)
    SET (CMAKE_INSTALL_PREFIX "/usr")
ENDIF (NOT DEFINED CMAKE_INSTALL_PREFIX)
MESSAGE (STATUS "Using CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_LIB_PREFIX)
    SET (CMAKE_INSTALL_LIB_PREFIX "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF (NOT DEFINED CMAKE_INSTALL_LIB_PREFIX)
MESSAGE (STATUS "Using CMAKE_INSTALL_LIB_PREFIX=${CMAKE_INSTALL_LIB_PREFIX}")
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_LIB_EXEC_DIR)
    SET (CMAKE_INSTALL_LIB_EXEC_DIR "${CMAKE_INSTALL_LIB_PREFIX}/${EXEC}")
ENDIF (NOT DEFINED CMAKE_INSTALL_LIB_EXEC_DIR)
MESSAGE (STATUS "Using CMAKE_INSTALL_LIB_EXEC_DIR=${CMAKE_INSTALL_LIB_EXEC_DIR}")
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_BONOBO_DIR)
    SET (CMAKE_INSTALL_BONOBO_DIR "${CMAKE_INSTALL_LIB_PREFIX}/bonobo/servers")
ENDIF (NOT DEFINED CMAKE_INSTALL_BONOBO_DIR)
MESSAGE (STATUS "Using CMAKE_INSTALL_BONOBO_DIR=${CMAKE_INSTALL_BONOBO_DIR}")
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_GCONF_SCHEMA_DIR)
    SET(CMAKE_INSTALL_GCONF_SCHEMA_DIR "${CMAKE_INSTALL_PREFIX}/share/gconf/schemas")
ENDIF(NOT DEFINED CMAKE_INSTALL_GCONF_SCHEMA_DIR)
MESSAGE (STATUS "Using CMAKE_INSTALL_GCONF_SCHEMA_DIR=${CMAKE_INSTALL_GCONF_SCHEMA_DIR}")
################################################################################
IF (NOT DEFINED CMAKE_INSTALL_BUILDER_UI_DIR)
    SET(CMAKE_INSTALL_BUILDER_UI_DIR "${CMAKE_INSTALL_PREFIX}/share/${EXEC}/builder")
ENDIF(NOT DEFINED CMAKE_INSTALL_BUILDER_UI_DIR)
MESSAGE (STATUS "Using CMAKE_INSTALL_BUILDER_UI_DIR=${CMAKE_INSTALL_BUILDER_UI_DIR}\n")
################################################################################

# Compile time options
SET (ENABLE_CONTEXT_MENU TRUE)
#SET (ENABLE_HELP_DOC TRUE)

ADD_SUBDIRECTORY (data)
ADD_SUBDIRECTORY (pixmaps)
ADD_SUBDIRECTORY (src)

# Uninstall target
CONFIGURE_FILE ("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET (uninstall
                   "${CMAKE_COMMAND}" -P 
                   "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Target to generate a distribution-ready tarball
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "File Browser Applet")
SET (CPACK_PACKAGE_VENDOR "Axel von Bertoldi")
SET (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET (CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
SET (CPACK_GENERATOR TGZ)
SET (CPACK_SOURCE_PACKAGE_FILE_NAME
    "${EXEC}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET (CPACK_SOURCE_GENERATOR TGZ)
SET (CPACK_SOURCE_IGNORE_FILES
     "GNOME_FileBrowserApplet_Factory.server$"
     "config.h$"
     ".svn"
     "build"
     "doc"
     "po")
INCLUDE (CPack)

ADD_CUSTOM_TARGET (deb checkinstall -D --pkgname="${EXEC}"
                                       --pkgversion="${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}"
                                       --install=0
                                       --deldoc=1
                                       --backup=0
                                       --pkglicense="GPLv2"
                                       --maintainer="bertoldia@gmail.com"
                                       --pkggroup="GNOME Desktop Environment")

ADD_CUSTOM_TARGET (really-clean rm -fr CMakeCache.txt
                                       cmake_install.cmake
                                       ${CPACK_SOURCE_PACKAGE_FILE_NAME}.tar.gz
                                       *.deb
                                       CPackConfig.cmake
                                       cmake_uninstall.cmake
                                       CMakeFiles
                                       CPackSourceConfig.cmake
                                       install_manifest.txt
                                       Makefile
                                       _CPack_Packages
                                       pixmaps/CMakeFiles
                                       pixmaps/cmake_install.cmake
                                       pixmaps/Makefile
                                       src/${EXEC}
                                       src/cmake_install.cmake
                                       src/config.h
                                       src/CMakeFiles
                                       src/Makefile
                                       data/CMakeFiles
                                       data/cmake_install.cmake
                                       data/GNOME_FileBrowserApplet_Factory.server
                                       data/Makefile) 

ADD_CUSTOM_TARGET (xgettext xgettext ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c --output-dir=${CMAKE_CURRENT_SOURCE_DIR}/po --from-code="UTF-8" -C --keyword="_")
