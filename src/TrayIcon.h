/*
 * Copyright 2011 Axel von Bertoldi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301, USA.
 */

#ifndef TRAY_ICON
#define TRAY_ICON

#include <gtkmm.h>
#include "DirectoryListing.h"

namespace FileBrowserApplet {

class TrayIcon : public Gtk::StatusIcon {
  private:
    static TrayIcon* instance;
    DirectoryListing* browser_menu;
    Gtk::Menu* meta_menu;

    void on_tray_icon_activate();
    void on_popup_menu(guint button, guint32 activate_time);
    void init_browser_menu();
    void init_meta_menu();
    void on_quit();
    void on_about();
    void connect_hotkey();
#ifndef LIBGTKHOTKEY_FOUND
    static void on_hotkey_pressed();
#endif
    TrayIcon();

  public:
    static TrayIcon& getInstance();
    static void deleteInstance();
    ~TrayIcon();
    int main(int argc, char** argv);
    void popdown();
};

} //namespace

#endif
