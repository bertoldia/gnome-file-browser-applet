#include <iostream>
#include <pangomm.h>
#include "BaseItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

int const MAX_FILE_NAME_LENGTH(30);

BaseItem::BaseItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  Gtk::ImageMenuItem(get_display_name_string(file_info->get_display_name()), true),
  file_info(file_info),
  path(path),
  collate_key(create_collate_key(file_info->get_display_name())) {

  init();
}

BaseItem::~BaseItem(){}

void
BaseItem::init() {
  Preferences prefs = Preferences::getInstance();
  add_image();
  if (prefs.show_tooltips()) add_tooltip();
  add_markup();
  set_ellipsize();
  connect_signals();
}

std::string
BaseItem::get_display_name_string(std::string display_name) {
  Glib::RefPtr<Glib::Regex> regex = Glib::Regex::create("_");
  return "_" + regex->replace_literal(display_name, 0, "__", Glib::REGEX_MATCH_NOTEMPTY);
}

void
BaseItem::_set_image(Gtk::Image* image) {
  set_always_show_image(true);
  manage(image);
  set_image(*image);
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
BaseItem::get_image_for_mime_type() {
  Gtk::Image* image = new Gtk::Image();
  image->set(file_info->get_icon(), Gtk::ICON_SIZE_SMALL_TOOLBAR);
  return image;
}

void
BaseItem::set_ellipsize() {
  Gtk::Label* label = (Gtk::Label*)get_child();
  label->set_max_width_chars(MAX_FILE_NAME_LENGTH);
  label->set_ellipsize(Pango::ELLIPSIZE_MIDDLE);
}

void
BaseItem::bold() {
  Gtk::Label* label = (Gtk::Label*)get_child();
  std::string text = label->get_text();
  gchar* bolded_text = g_markup_printf_escaped ("<span weight=\"bold\">%s</span>", text.c_str());
  label->set_markup(bolded_text);
  g_free(bolded_text);
}

} //namespace
