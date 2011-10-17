/*
 * Copyright 2011 Axel von Bertoldi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include <pangomm.h>

#include "Items.h"
#include "DirectoryListing.h"
#include "PanelMenuBar.h"
#include "Preferences.h"
#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

int const MAX_FILE_NAME_LENGTH(30);

/*************************** BaseItem ******************************************/
class BaseItem : public ImageMenuItem {
  private:
    string collate_key;
    string create_collate_key(const string& display_name);
    string get_display_name_string(string display_name);
    void set_ellipsize();

  protected:
    RefPtr<FileInfo> file_info;
    string path;

    virtual void add_image(){};
    virtual void add_tooltip(){};
    virtual void add_markup(){};
    virtual void connect_signals(){};

    Image* get_image_for_mime_type();
    void _set_image(Image* image);
    void bold();

  public:
    explicit BaseItem(const RefPtr<FileInfo>& file_info, const string& path);
    virtual ~BaseItem(){};
    void init();
    const string& get_collate_key();
};

BaseItem::BaseItem(const RefPtr<FileInfo>& file_info, const string& path) :
  ImageMenuItem(get_display_name_string(file_info->get_display_name()), true),
  file_info(file_info),
  path(path),
  collate_key(create_collate_key(file_info->get_display_name())) {

  init();
}

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
/*************************** FileItem ******************************************/
class FileItem : public BaseItem {
  private:
    Image* get_image_for_thumbnail();

  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void add_markup();
    virtual void connect_signals();

    void on_activate_file_item();
    bool on_button_release(const GdkEventButton* event);
    virtual bool on_middle_click();

  public:
    explicit FileItem(const RefPtr<FileInfo>& file_info,
                      const string& path);
    virtual ~FileItem();
};

FileItem::FileItem(const RefPtr<FileInfo>& file_info, const std::string& path) :
  BaseItem(file_info, path) {
}

FileItem::~FileItem(){}

Image*
FileItem::get_image_for_thumbnail() {
  std::string thumbnail = file_info->get_attribute_byte_string(G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
  if (!thumbnail.empty()) {
    return new Image(thumbnail);
  }
  return NULL;
}

void
FileItem::add_image() {
  Image* image = NULL;

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
      on_middle_click();
      return true;
    default:
      return false;
  }
}

bool
FileItem::on_middle_click() {
  PanelMenuBar::getInstance().deactivate();
  return open_file_with_app(Preferences::getInstance().get_alt_file_action(), path);
}
/*************************** DesktopItem ***************************************/
class DesktopItem : public FileItem {
  private:
    RefPtr<AppInfo> appinfo;
    Image* get_image_for_desktop_file();

  protected:
    virtual void add_image();
    virtual void add_markup();
    virtual void connect_signals();

    void on_activate_desktop_item();

  public:
    explicit DesktopItem(const RefPtr<FileInfo>& file_info,
                         const string& path,
                         const RefPtr<AppInfo>& appinfo);
    virtual ~DesktopItem();
};

DesktopItem::DesktopItem(const Glib::RefPtr<Gio::FileInfo>& file_info,
                         const std::string& path,
                         const Glib::RefPtr<Gio::AppInfo>& appinfo) :
  FileItem(file_info, path),
  appinfo(appinfo) {
  set_label(appinfo->get_name());
}

DesktopItem::~DesktopItem(){}

Image*
DesktopItem::get_image_for_desktop_file() {
  Image* image = new Image();
  image->set(appinfo->get_icon(), ICON_SIZE_SMALL_TOOLBAR);
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
  open_file_with_app(appinfo, "");
}
/*************************** DirectoryItem *************************************/
class DirectoryItem : public BaseItem {
  private:
    DirectoryListing* listing;

    void add_directory_submenu();
    void on_activate();

  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void connect_signals();

  public:
    explicit DirectoryItem(const RefPtr<FileInfo>& file_info,
                           const string& path);
    virtual ~DirectoryItem();
};

DirectoryItem::DirectoryItem(const RefPtr<FileInfo>& file_info,
                             const string& path) :
  BaseItem(file_info, path),
  listing(NULL) {

  add_directory_submenu();
}

DirectoryItem::~DirectoryItem(){}

void
DirectoryItem::add_image() {
  Image* image(get_image_for_mime_type());
  _set_image(image);
}

void
DirectoryItem::add_tooltip() {
}

void
DirectoryItem::connect_signals() {
  signal_activate().connect(sigc::mem_fun(this, &DirectoryItem::on_activate));
}

void
DirectoryItem::add_directory_submenu() {
  listing = manage(new DirectoryListing(path));
  set_submenu(*listing);
}

void
DirectoryItem::on_activate() {
  listing->refresh(file_info);
}
/*************************** MenuHeader ****************************************/
class MenuHeader : public FileItem {
  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void add_markup(){};

    virtual bool on_middle_click();

  public:
    explicit MenuHeader(const RefPtr<FileInfo>& file_info,
                        const string& path,
                        const int children_count);
    virtual ~MenuHeader();
};

MenuHeader::MenuHeader(const RefPtr<FileInfo>& file_info, const string& path) :
  FileItem(file_info, path) {
}

MenuHeader::~MenuHeader(){}

void
MenuHeader::add_image() {
  Image* image(get_image_for_mime_type());
  _set_image(image);
}

void
MenuHeader::add_tooltip() {
}

bool
MenuHeader::on_middle_click() {
  return open_file_with_app(Preferences::getInstance().get_alt_directory_action(), path);
}

void
MenuHeader::set_tooltip_item_count(int count) {
  std::stringstream tooltip;
  tooltip << file_info->get_display_name();
  tooltip << " - ";
  tooltip << count;
  tooltip << " items";
  set_tooltip_text(tooltip.str());
}
/*************************** MenuBrowser ***************************************/
class MenuBrowser : public DirectoryItem {
  public:
    explicit MenuBrowser(const RefPtr<FileInfo>& file_info,
                         const string& path,
                         const string& label);

    //void set_path(const string& path);
    //void set_label(const string& path);
    //void set_show_icon(bool show);
};

MenuBrowser::MenuBrowser(const RefPtr<FileInfo>& file_info,
                         const string& path,
                         const string& label) :
  DirectoryItem(File::create_for_path(path)->query_info(), path) {
  set_label(label);
}
/*************************** Factories *****************************************/
ImageMenuItem*
makeItem2(const RefPtr<FileInfo>& file_info,
         const string& path) {
  BaseItem* item = NULL;

  if (file_is_directory(file_info)) {
    item = new DirectoryItem(file_info, path);
  } else {
    RefPtr<AppInfo> appinfo = DesktopAppInfo::create_from_filename(path);
    if (appinfo) {
      item = new DesktopItem(file_info, path, appinfo);
    } else {
      item = new FileItem(file_info, path);
    }
  }
  item->init();
  return item;
}

ImageMenuItem*
makeMenuHeader(const RefPtr<FileInfo>& file_info,
               const string& path,
               const int children_count) {
  BaseItem* item = new MenuHeader(file_info, path);
  item->init();
  return item;
}

ImageMenuItem*
makeMenuBrowser(const RefPtr<FileInfo>& file_info,
                const string& path,
                const string& label) {
  BaseItem* item = new MenuBrowser(file_info, path, label);
  item->init();
  return item;
}
/*******************************************************************************/

} //namespace
