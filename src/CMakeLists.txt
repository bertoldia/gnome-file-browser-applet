INCLUDE (FindPkgConfig)

PKG_CHECK_MODULES (DEPS REQUIRED gtkmm-2.4>=2.20 giomm-2.4>=2.25 libpanelapplet-2.0)

INCLUDE_DIRECTORIES (${DEPS_INCLUDE_DIRS})
LINK_DIRECTORIES (${DEPS_LIBRARY_DIRS})
SET (LIBS ${DEPS_LIBRARIES})

OPTION (ENABLE_GTK_HOTKEY "Enable gtkhotkey library" ON)

IF (ENABLE_GTK_HOTKEY)
    PKG_CHECK_MODULES (LIBGTKHOTKEY gtkhotkey-1.0)

    IF (LIBGTKHOTKEY_FOUND)
        INCLUDE_DIRECTORIES (${LIBGTKHOTKEY_INCLUDE_DIRS})
        LINK_DIRECTORIES (${LIBGTKHOTKEY_LIBRARY_DIRS})
        SET (LIBS ${LIBS} ${LIBGTKHOTKEY_LIBRARIES})
        MESSAGE (STATUS "libgtkhotkey found at ${LIBGTKHOTKEY_LIBDIR}. Keyboard hotkey enabled.")
        SET (ENABLE_GTK_HOTKEY TRUE)
    ELSE (LIBGTKHOTKEY_FOUND)
        MESSAGE (STATUS "libgtkhotkey not found. Keyboard hotkey disabled.")
    ENDIF (LIBGTKHOTKEY_FOUND)

ELSE (ENABLE_GTK_HOTKEY)
    MESSAGE (STATUS "libgtkhotkey manually disabled.")
    SET (LIBGTKHOTKEY_FOUND FALSE)
ENDIF (ENABLE_GTK_HOTKEY)
MESSAGE("")

#CONFIGURE_FILE ("${FILE_BROWSER_APPLET_SOURCE_DIR}/src/config.h.in"
                #"${FILE_BROWSER_APPLET_SOURCE_DIR}/src/config.h")

SET (SRCS main.cc
     PanelMenuBar.cc
     BaseItem.cc
     FileItem.cc
     DirectoryItem.cc
     MenuHeader.cc
     DirectoryListing.cc
     DesktopItem.cc
     Preferences.cc
     Utils.cc
     MenuBrowser.cc)

ADD_EXECUTABLE (${EXEC} ${SRCS})
TARGET_LINK_LIBRARIES (${EXEC} ${LIBS})

INSTALL (TARGETS ${EXEC} DESTINATION "${CMAKE_INSTALL_LIB_EXEC_DIR}")
