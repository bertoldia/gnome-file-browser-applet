#include <iostream>
#include "FileItem.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

FileItem::FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) :
  Gtk::ImageMenuItem(file_info->get_display_name()),
  file_info(file_info),
  path(path),
  collate_key(create_collate_key(file_info->get_display_name())),
  listing(NULL),
  am_directory(file_is_directory(file_info)) {

  create();
}

FileItem::~FileItem(){}

void
FileItem::create() {
  add_image();
  add_markup();
  add_tooltip();
  add_directory_submenu();
}

void
FileItem::add_tooltip(){
  std::stringstream size;
  size << file_info->get_size();
  set_tooltip_text(size.str());
}

void
FileItem::add_markup(){}

void
FileItem::add_image(){
  Preferences* prefs = Preferences::getInstance();
  Gtk::Image* image = NULL;

  if (prefs->show_thumbnails()) {
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

// if the file is a directory
void
FileItem::add_directory_submenu() {
  if (!am_directory) return;

  listing = manage(new DirectoryListing(path));
  set_submenu(*listing);

  //signal_activate().connect(sigc::mem_fun(this, &FileItem::on_activate));
  _signal_activate = signal_activate().connect(sigc::mem_fun(this, &FileItem::on_activate));
}

void
FileItem::on_activate() {
  if (listing != NULL) {
    listing->refresh(file_info);
  }
}

bool
FileItem::is_directory() {
  return am_directory;
}

/* A little C nastiness... I really want to use
 * g_utf8_collate_key_for_filename() to sort the entries, but it's not exposed
 * in Glibmm, so I have to all-back to plain old glib.
 */
std::string
FileItem::create_collate_key(const std::string& display_name) {
  gchar* tmp = g_utf8_collate_key_for_filename(display_name.c_str(), display_name.size());
  std::string collate_key(tmp);
  g_free(tmp);
  return collate_key;
}

const std::string&
FileItem::get_collate_key() {
  return collate_key;
}

}
