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
  BaseItem(file_info, path) {
}

FileItem::~FileItem(){}

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
}


} //namespace
