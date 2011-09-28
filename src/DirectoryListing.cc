#include <iostream>
#include <algorithm>
#include <vector>

#include "DirectoryListing.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

bool
file_collate_comapator(BaseItem* A, BaseItem* B) {
  return A->get_collate_key() < B->get_collate_key();
}

DirectoryListing::DirectoryListing(const string& path) :
  path(path),
  header(NULL) {
}

DirectoryListing::~DirectoryListing(){}

void
DirectoryListing::refresh(const RefPtr<FileInfo>& file_info) {
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
  RefPtr<File> directory = File::create_for_path(path);
  try {
    RefPtr<FileEnumerator> children = directory->enumerate_children();
    add_children_entries(children);
    children->close();
  } catch (Gio::Error e) {
    cout << e.what() << endl;
    MenuItem* item = manage(new MenuItem(e.what()));
    item->set_sensitive(false);
    append(*item);
  }
}

void
DirectoryListing::add_children_entries(const RefPtr<FileEnumerator>& children) {
  Preferences& prefs = Preferences::getInstance();
  vector<BaseItem*> files;
  vector<BaseItem*> directories;

  RefPtr<FileInfo> child_info;
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

  header->set_tooltip_item_count(directories.size() + files.size());

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
DirectoryListing::add_header(const RefPtr<FileInfo>& file_info) {
  header = manage(MenuHeader::make(file_info, path));
  append((MenuItem&)*header);
  add_separator();
}

void
DirectoryListing::add_separator() {
  SeparatorMenuItem* separator = manage(new SeparatorMenuItem());
  append(*separator);
}

void
DirectoryListing::add_empty_item() {
  MenuItem* item = manage(new MenuItem("(Empty)"));
  item->set_sensitive(false);
  append(*item);
}

void
DirectoryListing::add_items(vector<BaseItem*> items) {
  sort (items.begin(), items.end(), file_collate_comapator);

  for (vector<BaseItem*>::iterator it = items.begin(); it != items.end(); it++) {
      append((MenuItem&)*(*it));
  }
}

} //namespace
