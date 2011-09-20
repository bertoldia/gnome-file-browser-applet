#include <iostream>
#include "DesktopItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

DesktopItem*
DesktopItem::make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) {
  DesktopItem* item = new DesktopItem(file_info, path);
  item->init();
  return item;
}

DesktopItem::DesktopItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  FileItem(file_info, path),
  appinfo(Gio::DesktopAppInfo::create_from_filename(path)) {
}

DesktopItem::~DesktopItem(){}

Gtk::Image*
DesktopItem::get_image_for_desktop_file() {

  set_label(appinfo->get_name()); //FIXME: this doesn't belong here.

  Gtk::Image* image = new Gtk::Image();
  image->set(appinfo->get_icon(), Gtk::ICON_SIZE_SMALL_TOOLBAR);
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
  Glib::ListHandle<std::string> files();
  Glib::RefPtr<Gio::AppLaunchContext> launch_context();
  appinfo->launch(files, launch_context);
}

bool
DesktopItem::on_button_release(GdkEventButton* event) {
  switch (event->button) {
    case 3:
      return true;
    default:
      return false;
  }
}

} //namespace
