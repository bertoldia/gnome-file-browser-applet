#include "MenuHeader.h"

namespace FileBrowserApplet {

MenuHeader::MenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  FileItem(file_info, path) {

}

MenuHeader::~MenuHeader(){}

void
MenuHeader::remove_directory_submenu() {
}

}
