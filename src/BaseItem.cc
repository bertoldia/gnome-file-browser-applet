#include <iostream>
#include <pangomm.h>

#include "BaseItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

int const MAX_FILE_NAME_LENGTH(30);

BaseItem::BaseItem(const RefPtr<FileInfo>& file_info, const string& path) :
  ImageMenuItem(get_display_name_string(file_info->get_display_name()), true),
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

string
BaseItem::get_display_name_string(string display_name) {
  RefPtr<Regex> regex = Regex::create("_");
  return "_" + regex->replace_literal(display_name, 0, "__", REGEX_MATCH_NOTEMPTY);
}

void
BaseItem::_set_image(Image* image) {
  set_always_show_image(true);
  manage(image);
  set_image(*image);
}

/* A little C nastiness... I really want to use
 * g_utf8_collate_key_for_filename() to sort the entries, but it's not exposed
 * in Glibmm, so I have to fall-back to plain old glib.
 */
string
BaseItem::create_collate_key(const string& display_name) {
  gchar* tmp = g_utf8_collate_key_for_filename(display_name.c_str(), display_name.size());
  string collate_key(tmp);
  g_free(tmp);
  return collate_key;
}

const string&
BaseItem::get_collate_key() {
  return collate_key;
}

Image*
BaseItem::get_image_for_mime_type() {
  Image* image = new Image();
  image->set(file_info->get_icon(), ICON_SIZE_SMALL_TOOLBAR);
  return image;
}

void
BaseItem::set_ellipsize() {
  Label* label = (Label*)get_child();
  label->set_max_width_chars(MAX_FILE_NAME_LENGTH);
  label->set_ellipsize(Pango::ELLIPSIZE_MIDDLE);
}

void
BaseItem::bold() {
  Label* label = (Label*)get_child();
  string text = label->get_text();
  gchar* bolded_text = g_markup_printf_escaped ("<span weight=\"bold\">%s</span>", text.c_str());
  label->set_markup(bolded_text);
  g_free(bolded_text);
}

} //namespace
