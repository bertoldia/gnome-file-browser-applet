#include <iostream>
#include "FileItem.h"
#include "Preferences.h"

FileItem::FileItem(Glib::RefPtr<Gio::FileInfo> file_info, std::string path) :
  Gtk::ImageMenuItem(file_info->get_display_name()),
  file_info(file_info),
  path(path),
  listing(NULL),
  am_directory(Gio::FILE_TYPE_DIRECTORY == file_info->get_file_type()) {

  create();
}

FileItem::~FileItem(){}

void
FileItem::create() {
  add_image();
  add_markup();
  add_tooltip();
  add_directory_submenu();
  add_signal();
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
}

void
FileItem::add_signal() {
  if (am_directory) {
    signal_activate().connect(sigc::mem_fun(this, &FileItem::on_activate));
  } else {
  }
}

void
FileItem::on_activate() {
  if (listing != NULL) {
    listing->refresh(file_info);
  }
}
