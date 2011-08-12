#include "MenuHeader.h"

MenuHeader::MenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  FileItem(file_info, path) {

  remove_directory_submenu();
}

MenuHeader::~MenuHeader(){}

void
MenuHeader::remove_directory_submenu() {
  remove_submenu();
  listing = NULL;

  _signal_activate.disconnect();
}
