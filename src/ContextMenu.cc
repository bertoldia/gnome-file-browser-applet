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

#include <iostream>

#include "ContextMenu.h"
#include "PanelMenuBar.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Gio;
using namespace Glib;
using namespace Gtk;

ContextMenu::ContextMenu(const string& path, MenuItem& parent_menu_item):
  path(path),
  parent_menu_item(parent_menu_item),
  file(File::create_for_path(path)),
  file_info(file->query_info()) {
  add_trash_item();

  signal_deactivate().connect(sigc::mem_fun(this, &ContextMenu::cleanup));
  show_all();
}

ContextMenu::~ContextMenu() {
}

void
ContextMenu::add_trash_item() {
  ImageMenuItem* item = manage(new ImageMenuItem(Stock::DELETE));
  append(*item);

  cout << "adding trash for " << path << endl;
}

void
ContextMenu::pop_up(const guint button,
                    const guint32 time) {
  tree_set_sensitive(false);
  popup(button, time);
}

void
ContextMenu::tree_set_sensitive(gboolean sensitive) {
  Widget* menu_shell = parent_menu_item.get_parent();
  while (menu_shell) {
    menu_shell->set_sensitive(sensitive);
    menu_shell = ((MenuShell*)menu_shell)->get_parent_shell();
  }
}

void
ContextMenu::cleanup() {
  tree_set_sensitive(true);
}

} //namespace
