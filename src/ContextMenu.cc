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

namespace FileBrowserApplet {

using namespace std;
using namespace Gio;
using namespace Glib;
using namespace Gtk;

ContextMenu::ContextMenu(const string& path):
  path(path),
  file(File::create_for_path(path)),
  file_info(file->query_info()) {
  add_trash_item();

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

} //namespace
