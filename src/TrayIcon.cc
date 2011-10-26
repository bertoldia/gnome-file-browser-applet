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

#include <giomm.h>
#include "TrayIcon.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Gio;
using namespace Glib;
using namespace Gtk;

TrayIcon::TrayIcon() {
  set("user-home");
  signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_tray_icon_activate));

  string path = get_home_dir();
  dir_menu = manage(new DirectoryListing(File::create_for_path(path)->query_info(), path));

}

void
TrayIcon::on_tray_icon_activate() {
  dir_menu->refresh();
  popup_menu_at_position(*dir_menu, 0, gtk_get_current_event_time());
}


} //namespace

