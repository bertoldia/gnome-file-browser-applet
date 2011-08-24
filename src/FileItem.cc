#include <iostream>
#include "FileItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

FileItem::FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  BaseItem(file_info, path) {

  create();
}

FileItem::~FileItem(){}

void
FileItem::create() {
}

} //namespace
