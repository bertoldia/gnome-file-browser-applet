#include <iostream>
#include "FileItem.h"
#include "Preferences.h"
#include "Utils.h"
#include "PanelMenuBar.h"

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

  if (Preferences::getInstance().show_thumbnails()) {
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
  if (file_info->get_attribute_boolean (G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE)) {
    bold();
  }
}

void
FileItem::connect_signals() {
  signal_button_release_event().connect(sigc::mem_fun(this, &FileItem::on_button_release));
  signal_activate().connect(sigc::mem_fun(this, &FileItem::on_activate_file_item));
}

void
FileItem::on_activate_file_item() {
  open_file(path);
}

bool
FileItem::on_button_release(const GdkEventButton* event) {
  switch (event->button) {
    case 3:
      return true;
    case 2:
      open_file_with_app(Preferences::getInstance().get_alt_file_action(), path);
      PanelMenuBar::getInstance().deactivate();
      return true;
    default:
      return false;
  }
}

} //namespace
