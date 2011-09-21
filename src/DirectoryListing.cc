#include <iostream>
#include <algorithm>
#include <vector>
#include "DirectoryListing.h"
#include "FileItem.h"
#include "DirectoryItem.h"
#include "MenuHeader.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

bool
file_collate_comapator(BaseItem* A, BaseItem* B) {
  return A->get_collate_key() < B->get_collate_key();
}

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
  Preferences& prefs = Preferences::getInstance();
  std::vector<BaseItem*> files;
  std::vector<BaseItem*> directories;

  Glib::RefPtr<Gio::FileInfo> child_info;
  while (child_info = children->next_file()) {

    if ((!prefs.show_hidden() && child_info->is_hidden()) ||
        (prefs.show_dirs_only() && !file_is_directory(child_info))) {
      continue;
    }

    BaseItem* item = manage(makeItem(child_info, path + "/" + child_info->get_name()));
    if (file_is_directory(child_info)) {
      directories.push_back(item);
    } else {
      files.push_back(item);
    }
  }

  update_related_object_tooltips(directories.size() + files.size());

  if (directories.empty() && files.empty()) {
    add_empty_item();
    return;
  }

  add_items(directories);

  if (!(directories.empty() || files.empty())) {
    add_separator();
  }

  add_items(files);
}

void
DirectoryListing::add_header(const Glib::RefPtr<Gio::FileInfo>& file_info) {
  MenuHeader* header = manage(MenuHeader::make(file_info, path));
  append((Gtk::MenuItem&)*header);
  add_separator();
}

void
DirectoryListing::add_separator() {
  Gtk::SeparatorMenuItem* separator = manage(new Gtk::SeparatorMenuItem());
  append(*separator);
}

void
DirectoryListing::add_empty_item() {
  Gtk::MenuItem* item = manage(new Gtk::MenuItem("(Empty)"));
  item->set_sensitive(false);
  append(*item);
}

void
DirectoryListing::add_items(std::vector<BaseItem*> items) {
  sort (items.begin(), items.end(), file_collate_comapator);

  for (std::vector<BaseItem*>::iterator it = items.begin(); it != items.end(); it++) {
      append((Gtk::MenuItem&)*(*it));
  }
}

void
DirectoryListing::update_related_object_tooltips(int items) {
}

} //namespace
