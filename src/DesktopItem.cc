#include <iostream>

#include "DesktopItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

DesktopItem*
DesktopItem::make(const Glib::RefPtr<Gio::FileInfo>& file_info,
                  const std::string& path,
                  const Glib::RefPtr<Gio::AppInfo>& appinfo) {
  DesktopItem* item = new DesktopItem(file_info, path, appinfo);
  item->init();
  return item;
}

DesktopItem::DesktopItem(const Glib::RefPtr<Gio::FileInfo>& file_info,
                         const std::string& path,
                         const Glib::RefPtr<Gio::AppInfo>& appinfo) :
  FileItem(file_info, path),
  appinfo(appinfo) {
  set_label(appinfo->get_name());
}

DesktopItem::~DesktopItem(){}

Image*
DesktopItem::get_image_for_desktop_file() {
  Image* image = new Image();
  image->set(appinfo->get_icon(), ICON_SIZE_SMALL_TOOLBAR);
  return image;
}

void
DesktopItem::add_image() {
  _set_image(get_image_for_desktop_file());
}

void
DesktopItem::add_markup() {
  bold();
}

void
DesktopItem::connect_signals() {
  signal_button_release_event().connect(sigc::mem_fun(this, &DesktopItem::on_button_release));
  signal_activate().connect(sigc::mem_fun(this, &DesktopItem::on_activate_desktop_item));
}

void
DesktopItem::on_activate_desktop_item() {
  open_file_with_app(appinfo, "");
}

} //namespace
