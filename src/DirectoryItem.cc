#include <iostream>
#include "DirectoryItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

DirectoryItem::DirectoryItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  ItemBase(file_info, path),
  listing(NULL) {

  create();
}

DirectoryItem::~DirectoryItem(){}

void
DirectoryItem::create() {
  add_directory_submenu();
}

void
DirectoryItem::add_directory_submenu() {
  listing = manage(new DirectoryListing(path));
  set_submenu(*listing);
  _signal_activate = signal_activate().connect(sigc::mem_fun(this, &DirectoryItem::on_activate));
}

void
DirectoryItem::on_activate() {
  listing->refresh(file_info);
}

}
