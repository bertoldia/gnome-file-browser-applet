#include "MenuHeader.h"

namespace FileBrowserApplet {

MenuHeader*
MenuHeader::make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) {
  MenuHeader* item = new MenuHeader(file_info, path);
  item->init();
  return item;
}

MenuHeader::MenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  FileItem(file_info, path) {
}

MenuHeader::~MenuHeader(){}

void
MenuHeader::add_image() {
  Gtk::Image* image(get_image_for_mime_type());
  _set_image(image);
}

void
MenuHeader::add_tooltip() {
}

} //namespace
