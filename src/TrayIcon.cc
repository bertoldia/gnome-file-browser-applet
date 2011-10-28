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

int
TrayIcon::main(int argc, char** argv) {
  Gtk::Main::run();
  return EXIT_SUCCESS;
}

TrayIcon::TrayIcon() {
  set("user-home");

  init_browser_menu();
  init_meta_menu();

  signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_tray_icon_activate));
  signal_popup_menu().connect(sigc::mem_fun(this, &TrayIcon::on_popup_menu));
}

TrayIcon::~TrayIcon() {
}

void
TrayIcon::on_tray_icon_activate() {
  browser_menu->refresh();
  popup_menu_at_position(*browser_menu, 0, gtk_get_current_event_time());
  browser_menu->select_first();
}

void
TrayIcon::on_popup_menu(guint button, guint32 activate_time) {
  popup_menu_at_position(*meta_menu, 0, gtk_get_current_event_time());
}

void
TrayIcon::init_browser_menu() {
  string path = get_home_dir();
  browser_menu = manage(new DirectoryListing(File::create_for_path(path)->query_info(), path));
}

void
TrayIcon::init_meta_menu() {
  meta_menu = manage(new Menu());

  ImageMenuItem* item;

  item = new ImageMenuItem(Stock::PREFERENCES);
  meta_menu->append(*item);

  item = new ImageMenuItem(Stock::ABOUT);
  meta_menu->append(*item);

  item = new ImageMenuItem(Stock::QUIT);
  meta_menu->append(*item);
  item->signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_quit));

  meta_menu->show_all();
}

void
TrayIcon::on_quit() {
  Gtk::Main::quit();
}

} //namespace
