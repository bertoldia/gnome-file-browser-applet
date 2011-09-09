#include <iostream>
#include "FileItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

FileItem*
FileItem::make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) {
  FileItem* item = new FileItem(file_info, path);
  item->init();
  return item;
}

FileItem::FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  BaseItem(file_info, path),
  is_desktop(false) {
}

FileItem::~FileItem(){}

Gtk::Image*
FileItem::get_image_for_desktop_file() {
  Glib::RefPtr<Gio::DesktopAppInfo> appinfo = Gio::DesktopAppInfo::create_from_filename(path);

  if (appinfo) {
    is_desktop = true;
    set_label(appinfo->get_name()); //FIXME: this doesn't belong here.

    Gtk::Image* image = new Gtk::Image();
    image->set(appinfo->get_icon(), Gtk::ICON_SIZE_SMALL_TOOLBAR);
    return image;
  }
  return NULL;
}

Gtk::Image*
FileItem::get_image_for_thumbnail() {
  std::string thumbnail = file_info->get_attribute_byte_string(G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
  if (!thumbnail.empty()) {
    return new Gtk::Image(thumbnail);
  }
  return NULL;
}

void
FileItem::add_image() {
  Gtk::Image* image = NULL;

  image = get_image_for_desktop_file();

  if (image == NULL && Preferences::getInstance().show_thumbnails()) {
    image = get_image_for_thumbnail();
  }

  if (image == NULL) {
    image = get_image_for_mime_type();
  }
  _set_image(image);
}

void
FileItem::add_tooltip() {
  std::string tooltip(file_info->get_display_name());
  tooltip += " - ";
  tooltip += get_file_size_string_from_size(file_info->get_size());
  set_tooltip_text(tooltip);
}

void
FileItem::add_markup() {
}

void
FileItem::connect_signals() {
  signal_button_release_event().connect(sigc::mem_fun(this, &FileItem::on_button_release));
  signal_activate().connect(sigc::mem_fun(this, &FileItem::on_activate_file_item));
}

void
FileItem::on_activate_file_item() {
  std::cout<< __FUNCTION__ << std::endl;
  open_file(path);
}

bool
FileItem::on_button_release(GdkEventButton* event) {
  std::cout<< __FUNCTION__ << std::endl;
  switch (event->button) {
    case 3:
      std::cout << "right click\n";
      return true;
    default:
      return false;
  }
}

} //namespace
