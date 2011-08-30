#include <iostream>
#include "BaseItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

BaseItem::BaseItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  Gtk::ImageMenuItem(file_info->get_display_name()),
  file_info(file_info),
  path(path),
  collate_key(create_collate_key(file_info->get_display_name())) {

  create();
}

BaseItem::~BaseItem(){}

void
BaseItem::create() {
  add_image();
  add_markup();
  add_tooltip();
}

void
BaseItem::add_tooltip(){
  if (!Preferences::getInstance().show_tooltips()) return;
  std::string tooltip(file_info->get_display_name());

  tooltip += " - ";
  tooltip += get_file_size_string_from_size(file_info->get_size());

  set_tooltip_text(tooltip);
}

void
BaseItem::add_markup(){}

void
BaseItem::add_image(){
  Gtk::Image* image = NULL;

  image = get_image_for_desktop_file();

  if (image == NULL && Preferences::getInstance().show_thumbnails()) {
    image = get_image_for_thumbnail();
  }

  if (image == NULL) {
    image = get_image_for_mime_type();
  }

  manage(image);
  set_image(*image);
  set_always_show_image(true);
}

/* A little C nastiness... I really want to use
 * g_utf8_collate_key_for_filename() to sort the entries, but it's not exposed
 * in Glibmm, so I have to fall-back to plain old glib.
 */
std::string
BaseItem::create_collate_key(const std::string& display_name) {
  gchar* tmp = g_utf8_collate_key_for_filename(display_name.c_str(), display_name.size());
  std::string collate_key(tmp);
  g_free(tmp);
  return collate_key;
}

const std::string&
BaseItem::get_collate_key() {
  return collate_key;
}

Gtk::Image*
BaseItem::get_image_for_desktop_file() {
  Glib::RefPtr<Gio::DesktopAppInfo> appinfo = Gio::DesktopAppInfo::create_from_filename(path);
  return NULL;
}

Gtk::Image*
BaseItem::get_image_for_thumbnail() {
  std::string thumbnail = file_info->get_attribute_byte_string(G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
  if (!thumbnail.empty()) {
    return new Gtk::Image(thumbnail);
  }
  return NULL;
}

Gtk::Image*
BaseItem::get_image_for_mime_type() {
  Gtk::Image* image = new Gtk::Image();
    image->set(file_info->get_icon(), Gtk::ICON_SIZE_SMALL_TOOLBAR);
    return image;
}

} //namespace
