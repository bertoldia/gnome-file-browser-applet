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

#ifndef CONTEXT_MENU
#define CONTEXT_MENU

#include <giomm.h>
#include <gtkmm.h>

#include "Utils.h"

namespace FileBrowserApplet {

class ContextMenu : public Gtk::Menu {
  private:
    std::string path;
    Glib::RefPtr<Gio::File> file;
    Glib::RefPtr<Gio::FileInfo> file_info;

    void add_trash_item();

  public:
    explicit ContextMenu(const std::string& path);
    ~ContextMenu();
};

} //namespace

#endif

