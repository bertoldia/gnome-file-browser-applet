#include <iostream>

#include "DirectoryItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

DirectoryItem*
DirectoryItem::make(const RefPtr<FileInfo>& file_info, const string& path) {
  DirectoryItem* item = new DirectoryItem(file_info, path);
  item->init();
  return item;
}

DirectoryItem::DirectoryItem(const RefPtr<FileInfo>& file_info, const string& path) :
  BaseItem(file_info, path),
  listing(NULL) {

  add_directory_submenu();
}

DirectoryItem::~DirectoryItem(){}

void
DirectoryItem::add_image() {
  Image* image(get_image_for_mime_type());
  _set_image(image);
}

void
DirectoryItem::add_tooltip() {
}

void
DirectoryItem::connect_signals() {
  signal_activate().connect(sigc::mem_fun(this, &DirectoryItem::on_activate));
}

void
DirectoryItem::add_directory_submenu() {
  listing = manage(new DirectoryListing(path));
  set_submenu(*listing);
}

void
DirectoryItem::on_activate() {
  listing->refresh(file_info);
}

} //namespace
