#include "MenuHeader.h"

MenuHeader::MenuHeader(Glib::RefPtr<Gio::FileInfo> file_info, std::string path) :
  FileItem(file_info, path) {
}

MenuHeader::~MenuHeader(){}

void
MenuHeader::add_directory_submenu() {
  // do nothing.
}
