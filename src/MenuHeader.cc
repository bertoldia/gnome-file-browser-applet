#include "MenuHeader.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

MenuHeader*
MenuHeader::make(const RefPtr<FileInfo>& file_info, const string& path) {
  MenuHeader* item = new MenuHeader(file_info, path);
  item->init();
  return item;
}

MenuHeader::MenuHeader(const RefPtr<FileInfo>& file_info, const string& path) :
  FileItem(file_info, path) {
}

MenuHeader::~MenuHeader(){}

void
MenuHeader::add_image() {
  Image* image(get_image_for_mime_type());
  _set_image(image);
}

void
MenuHeader::add_tooltip() {
}

bool
MenuHeader::on_middle_click() {
  return open_file_with_app(Preferences::getInstance().get_alt_directory_action(), path);
}

void
MenuHeader::set_tooltip_item_count(int count) {
  std::stringstream tooltip;
  tooltip << file_info->get_display_name();
  tooltip << " - ";
  tooltip << count;
  tooltip << " items";
  set_tooltip_text(tooltip.str());
}

} //namespace
