#include <iostream>
#include <giomm.h>
#include "DirectoryListing.h"
#include "FileItem.h"

DirectoryListing::DirectoryListing(std::string path) :
  path(path),
  item(NULL) {
}

DirectoryListing::~DirectoryListing(){}

void
DirectoryListing::refresh() {
  clear();
  _populate();
  show_all();
}

void
DirectoryListing::clear () {
  // Does this delete the menu items too, or just remove them from the menu?
  items().erase(items().begin(), items().end());
}

void
DirectoryListing::populate() {
  for (int i = 0; i < 10; i++) {
    item = manage(new Gtk::MenuItem("sdfsfsdF"));
    append(*item);
  }
}

void
DirectoryListing::_populate() {
  Glib::RefPtr<Gio::File> directory = Gio::File::create_for_path(path);
  try {
    Glib::RefPtr<Gio::FileEnumerator> children = directory->enumerate_children();
    Glib::RefPtr<Gio::FileInfo> child_info;
    while (child_info = children->next_file()) {
      FileItem* file_item = manage(new FileItem(child_info, path + "/" + child_info->get_name()));
      append((Gtk::MenuItem&)*file_item);
    }
  } catch (Gio::Error e) {
    std::cout << e.what() << std::endl;
  }
}

void
DirectoryListing::create(){}

void
DirectoryListing::add_header(){}

void
DirectoryListing::add_directories(){}

void
DirectoryListing::add_files(){}

