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

class ContextMenu : public IContextMenu {
  private:
  std::string path;
  Gtk::MenuItem& parent_menu_item;
  Glib::RefPtr<Gio::File> file;
  Glib::RefPtr<Gio::FileInfo> file_info;

  public:
  ContextMenu(const string& path, MenuItem& parent_menu_item):
    path(path),
    parent_menu_item(parent_menu_item),
    file(File::create_for_path(path)),
    file_info(file->query_info()) {

    add_trash_item();
    add_delete_item();

    signal_deactivate().connect(sigc::mem_fun(this, &ContextMenu::cleanup));
    show_all();
  }

  virtual ~ContextMenu() {
  }

  void
  pop_up(const guint button, const guint32 time) {
    tree_set_sensitive(false);
    popup(button, time);
  }

  private:
  void
  tree_set_sensitive(gboolean sensitive) {
    Widget* menu_shell = parent_menu_item.get_parent();
    while (menu_shell) {
      menu_shell->set_sensitive(sensitive);
      menu_shell = ((MenuShell*)menu_shell)->get_parent_shell();
    }
  }

  void
  cleanup() {
    tree_set_sensitive(true);
  }

  void
  add_trash_item() {
    Image* image = new Image();
    image->set_from_icon_name("stock_trash_full", ICON_SIZE_SMALL_TOOLBAR);
    ImageMenuItem* item = manage(new ImageMenuItem(*image, "Move To _Trash", true));
    append(*item);
  }

  void
  add_delete_item() {
    ImageMenuItem* item = manage(new ImageMenuItem("_Delete Permanently", true));
    item->set_image((Widget&)*(new Image(Stock::DELETE, ICON_SIZE_SMALL_TOOLBAR)));
    append(*item);
  }
};

IContextMenu*
makeContextMenu(const std::string& path, Gtk::MenuItem& parent_menu_item) {
  return new ContextMenu(path, parent_menu_item);
}

} //namespace