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

#ifndef ITEMS
#define ITEMS

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

Gtk::ImageMenuItem* makeItem2(const Glib::RefPtr<Gio::FileInfo>& file_info,
                             const std::string& path);
Gtk::ImageMenuItem* makeMenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info,
                                   const std::string& path,
                                   const int children_count);
Gtk::ImageMenuItem* makeMenuBrowser(const std::string& path,
                                    const std::string& label);

} //namespace

#endif
