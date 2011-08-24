#include <iostream>
#include "ItemBase.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

ItemBase::ItemBase(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  Gtk::ImageMenuItem(file_info->get_display_name()),
  file_info(file_info),
  path(path),
  collate_key(create_collate_key(file_info->get_display_name())) {

  create();
}

ItemBase::~ItemBase(){}

void
ItemBase::create() {
  add_image();
  add_markup();
  add_tooltip();
}

void
ItemBase::add_tooltip(){
  if (!Preferences::getInstance().show_tooltips()) return;
  std::string tooltip(file_info->get_display_name());

  tooltip += " - ";
  tooltip += get_file_size_string_from_size(file_info->get_size());

  set_tooltip_text(tooltip);
}

void
ItemBase::add_markup(){}

void
ItemBase::add_image(){
  Gtk::Image* image = NULL;

  if (Preferences::getInstance().show_thumbnails()) {
    std::string thumbnail = file_info->get_attribute_byte_string(G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
    if (!thumbnail.empty()) {
      image = manage(new Gtk::Image(thumbnail));
    }
  }

  if (image == NULL) {
    image = manage(new Gtk::Image());
    image->set(file_info->get_icon(), Gtk::ICON_SIZE_SMALL_TOOLBAR);
  }
  set_image(*image);
  set_always_show_image(true);
}

/* A little C nastiness... I really want to use
 * g_utf8_collate_key_for_filename() to sort the entries, but it's not exposed
 * in Glibmm, so I have to all-back to plain old glib.
 */
std::string
ItemBase::create_collate_key(const std::string& display_name) {
  gchar* tmp = g_utf8_collate_key_for_filename(display_name.c_str(), display_name.size());
  std::string collate_key(tmp);
  g_free(tmp);
  return collate_key;
}

const std::string&
ItemBase::get_collate_key() {
  return collate_key;
}

}

