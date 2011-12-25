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
#include "TrayIcon.h"

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
    add_open_with_item();

    //signal_deactivate().connect(sigc::mem_fun(this, &ContextMenu::cleanup));
    show_all();
  }

  virtual ~ContextMenu() {
  }

  void
  pop_up(const guint button, const guint32 time) {
    //tree_set_sensitive(false);
    popup(button, time);
  }

  private:
  void
  tree_set_sensitive(gboolean sensitive) {
    return;
    Widget* menu_shell = parent_menu_item.get_parent();
    while (menu_shell) {
      menu_shell->set_sensitive(sensitive);
      //menu_shell = ((MenuShell*)menu_shell)->get_parent_shell();
    }
  }

  void
  cleanup() {
    //tree_set_sensitive(true);
    //parent_menu_item.grab_focus();
    //parent_menu_item.get_parent()->grab_focus();

    TrayIcon::getInstance().popdown();
  }

  void
  add_trash_item() {
    Image* image = new Image();
    image->set_from_icon_name("stock_trash_full", ICON_SIZE_SMALL_TOOLBAR);
    ImageMenuItem* item = manage(new ImageMenuItem(*image, "Move To _Trash", true));
    append(*item);

    item->signal_activate().connect(sigc::mem_fun(this, &ContextMenu::on_trash_item_activate));
  }

  void
  on_trash_item_activate() {
    file->trash();
    if(file_is_directory(file_info)) {
      delete(((Menu*)parent_menu_item.get_parent())->get_attach_widget());
    } else {
      delete(&parent_menu_item);
    }
  }

  void
  add_delete_item() {
    ImageMenuItem* item = manage(new ImageMenuItem("_Delete Permanently", true));
    item->set_image((Widget&)*(new Image(Stock::DELETE, ICON_SIZE_SMALL_TOOLBAR)));
    append(*item);
  }

  void
  add_open_with_item() {
    SeparatorMenuItem* separator = manage(new SeparatorMenuItem());
    append(*separator);

    ImageMenuItem* item = manage(new ImageMenuItem("_Open With", true));
    item->set_image((Widget&)*(new Image(Stock::OPEN, ICON_SIZE_SMALL_TOOLBAR)));
    append(*item);

    Menu* menu = new Menu();
    item->set_submenu(*menu);

    vector<RefPtr<AppInfo> > apps = AppInfo::get_all_for_type(file_info->get_content_type());
    for (vector<RefPtr<AppInfo> >::iterator it = apps.begin(); it != apps.end(); it++) {
      RefPtr<AppInfo> app = *it;
      Image* image = new Image();
      image->set((RefPtr<const Icon>)app->get_icon(), ICON_SIZE_SMALL_TOOLBAR);
      item = new ImageMenuItem(app->get_name(), true);
      item->set_image(*image);
      menu->append(*item);
      item->signal_activate().connect(
          sigc::bind(sigc::mem_fun(this, &ContextMenu::on_open_with_app_activate), app));
    }

    separator = manage(new SeparatorMenuItem());
    append(*separator);
  }

  void
  on_open_with_app_activate(RefPtr<AppInfo> app) {
    open_file_with_app(app, path);
    cleanup();
  }
};

IContextMenu*
makeContextMenu(const std::string& path, Gtk::MenuItem& parent_menu_item) {
  return new ContextMenu(path, parent_menu_item);
}

} //namespace
