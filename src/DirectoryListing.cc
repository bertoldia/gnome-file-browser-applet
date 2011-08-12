#include <iostream>
#include <giomm.h>
#include "DirectoryListing.h"
#include "MenuHeader.h"
#include "Preferences.h"

DirectoryListing::DirectoryListing(const std::string& path) :
  path(path),
  _item(NULL) {
}

DirectoryListing::~DirectoryListing(){}

void
DirectoryListing::refresh(const Glib::RefPtr<Gio::FileInfo>& file_info) {
  clear();
  add_header(file_info);
  populate();
  show_all();
}

void
DirectoryListing::clear () {
  // Does this delete the menu items too, or just remove them from the menu?
  // It deletes them.
  items().erase(items().begin(), items().end());
}

void
DirectoryListing::populate() {
  query_file_system_sync();
}

void
DirectoryListing::query_file_system_sync() {
  Glib::RefPtr<Gio::File> directory = Gio::File::create_for_path(path);
  try {
    Glib::RefPtr<Gio::FileEnumerator> children = directory->enumerate_children();
    add_children_entries(children);
    children->close();
  } catch (Gio::Error e) {
    std::cout << e.what() << std::endl;
    Gtk::MenuItem* item = manage(new Gtk::MenuItem(e.what()));
    item->set_sensitive(false);
    append(*item);
  }
}

void
DirectoryListing::add_children_entries(const Glib::RefPtr<Gio::FileEnumerator>& children) {
  Preferences* prefs = Preferences::getInstance();

  Glib::RefPtr<Gio::FileInfo> child_info;
  while (child_info = children->next_file()) {
    if ((!prefs->show_hidden() && child_info->is_hidden()) ||
        (prefs->show_dirs_only() && (Gio::FILE_TYPE_DIRECTORY != child_info->get_file_type()))) continue;

    FileItem* file_item = manage(new FileItem(child_info, path + "/" + child_info->get_name()));
    append((Gtk::MenuItem&)*file_item);
  }
}

void
DirectoryListing::add_header(const Glib::RefPtr<Gio::FileInfo>& file_info) {
  MenuHeader* header = manage(new MenuHeader(file_info, path));
  append((Gtk::MenuItem&)*header);
  add_separator();
}

void
DirectoryListing::add_directories(){}

void
DirectoryListing::add_files(){}

void
DirectoryListing::add_separator() {
  Gtk::SeparatorMenuItem* separator = manage(new Gtk::SeparatorMenuItem());
  append(*separator);
}

