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

const unsigned int MAX_ITEMS_SHOW_SOFT(35) ;
const unsigned int MAX_ITEMS_SHOW_HARD(30) ;

bool
file_collate_comapator(IBaseItem* A, IBaseItem* B) {
  return A->get_collate_key() < B->get_collate_key();
}

DirectoryListing::DirectoryListing(const Glib::RefPtr<Gio::FileInfo>& file_info,
                                   const std::string& path) :
  path(path),
  file_info(file_info) {
}

DirectoryListing::~DirectoryListing() {
}

void
DirectoryListing::refresh() {
  clear();
  populate();
}

void
DirectoryListing::clear () {
  sigc::slot<void, Widget&> slot = mem_fun(this, &DirectoryListing::remove_child);
  foreach(slot);
}

void
DirectoryListing::remove_child(Widget& widget) {
  remove(widget);
  delete(&widget);
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
    add_error_item(e.what());
  }
}

void
DirectoryListing::add_children_entries(const RefPtr<FileEnumerator>& children) {
  Preferences& prefs = Preferences::getInstance();
  vector<IBaseItem*> files;
  vector<IBaseItem*> directories;

  RefPtr<FileInfo> child_info;
  while (child_info = children->next_file()) {

    if ((!prefs.show_hidden() && child_info->is_hidden()) ||
        (prefs.show_dirs_only() && !file_is_directory(child_info))) {
      continue;
    }

    IBaseItem* item = manage(makeItem(child_info, path + "/" + child_info->get_name()));
    if (file_is_directory(child_info)) {
      directories.push_back(item);
    } else {
      files.push_back(item);
    }
  }

  add_header(directories.size() + files.size());

  if (directories.empty() && files.empty()) {
    add_error_item("(Empty)");
    return;
  }

  add_items(directories);

  if (!(directories.empty() || files.empty())) {
    add_separator();
  }

  add_items(files);

  if (directories.size() + files.size() <= MAX_ITEMS_SHOW_SOFT) {
    show_all();
  } else {
    show_limited();
  }
}

void
DirectoryListing::add_header(const unsigned int children_count) {
  IBaseItem* header = manage(makeMenuHeader(file_info, path, children_count));
  header->show();
  append((Gtk::MenuItem&)*header);
  add_separator();
}

void
DirectoryListing::add_separator() {
  SeparatorMenuItem* separator = manage(new SeparatorMenuItem());
  separator->show();
  append(*separator);
}

void
DirectoryListing::add_error_item(const string& message) {
  Gtk::MenuItem* item = manage(new Gtk::MenuItem(message));
  item->show();
  item->set_sensitive(false);
  append(*item);
}

void
DirectoryListing::add_items(vector<IBaseItem*> items) {
  sort (items.begin(), items.end(), file_collate_comapator);

  for (vector<IBaseItem*>::iterator it = items.begin(); it != items.end(); it++) {
      append((Gtk::MenuItem&)*(*it));
  }
}

void
DirectoryListing::show_limited() {
  vector<Widget*> items = get_children();
  vector<Widget*>::iterator it;
  for (unsigned int i = 0; i < MAX_ITEMS_SHOW_SOFT; i++) {
    items[i]->show();
  }
  add_more_item();
}

void
DirectoryListing::add_more_item() {
  ImageMenuItem* more_item = manage(new ImageMenuItem(Stock::ADD));
  more_item->set_always_show_image(true);

  stringstream text;
  text << get_children().size() - MAX_ITEMS_SHOW_HARD << " more...";
  more_item->set_label(text.str());

  more_item->signal_button_release_event().connect(sigc::mem_fun(this, &DirectoryListing::on_button_release_more_item));
  signal_key_release_event().connect(sigc::mem_fun(this, &DirectoryListing::on_key_release_more_item));

  append(*more_item);
  more_item->show();
}

bool
DirectoryListing::on_button_release_more_item(const GdkEventButton* event) {
  return on_activate_more_item();
}

bool
DirectoryListing::on_key_release_more_item(const GdkEventKey* event) {
  return on_activate_more_item();
}

bool
DirectoryListing::on_activate_more_item() {
  Widget* more_menu_item = get_children().back();
  remove(*more_menu_item);
  delete(more_menu_item);
  show_all();
  return true;
}

} //namespace
