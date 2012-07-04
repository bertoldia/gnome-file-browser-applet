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
#include "TrayIcon.h"
#include "Preferences.h"
#include "Utils.h"
#include "ContextMenu.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

int const MAX_FILE_NAME_LENGTH(30);

/*************************** BaseItem ******************************************/
class BaseItem : public IBaseItem {
  private:
    string collate_key;

    /* A little C nastiness... I really want to use
     * g_utf8_collate_key_for_filename() to sort the entries, but it's not exposed
     * in Glibmm, so I have to fall-back to plain old glib.
     */
    string create_collate_key(const string& display_name) {
      gchar* tmp = g_utf8_collate_key_for_filename(display_name.c_str(), display_name.size());
      string collate_key(tmp);
      g_free(tmp);
      return collate_key;
    }

    string get_display_name_string(string display_name) {
      RefPtr<Regex> regex = Regex::create("_");
      return "_" + regex->replace_literal(display_name, 0, "__", REGEX_MATCH_NOTEMPTY);
    }

    void set_ellipsize() {
      Label* label = (Label*)get_child();
      if (label) {
        label->set_max_width_chars(MAX_FILE_NAME_LENGTH);
        label->set_ellipsize(Pango::ELLIPSIZE_MIDDLE);
      }
    }

  protected:
    RefPtr<FileInfo> file_info;
    string path;

    virtual void add_image() {
      Image* image(get_image_for_mime_type());
      _set_image(image);
    };

    virtual void add_tooltip(){};
    virtual void add_markup(){};
    virtual void connect_signals(){};

    Image* get_image_for_mime_type() {
      Image* image = new Image();
      image->set((RefPtr<const Icon>)file_info->get_icon(), ICON_SIZE_SMALL_TOOLBAR);
      return image;
    }

    void _set_image(Image* image) {
      set_always_show_image(true);
      manage(image);
      set_image(*image);
    }

    void bold() {
      Label* label = (Label*)get_child();
      if (label) {
        string text = label->get_text();
        gchar* bolded_text = g_markup_printf_escaped ("<span weight=\"bold\">%s</span>", text.c_str());
        label->set_markup(bolded_text);
        g_free(bolded_text);
      }
    }

  public:
    explicit BaseItem(const RefPtr<FileInfo>& file_info, const string& path) :
      IBaseItem(get_display_name_string(file_info->get_display_name())),
      collate_key(create_collate_key(file_info->get_display_name())),
      file_info(file_info),
      path(path) {
    }

    virtual ~BaseItem(){};

    void init() {
      Preferences prefs = Preferences::getInstance();
      add_image();
      if (prefs.show_tooltips()) add_tooltip();
      add_markup();
      set_ellipsize();
      connect_signals();
    }

    virtual const string& get_collate_key() {
      return collate_key;
    }
};
/*************************** FileItem ******************************************/
class FileItem : public BaseItem {
  private:
    IContextMenu* context_menu;

    Image* get_image_for_thumbnail() {
      std::string thumbnail = file_info->get_attribute_byte_string(G_FILE_ATTRIBUTE_THUMBNAIL_PATH);
      if (!thumbnail.empty()) {
        return new Image(thumbnail);
      }
      return NULL;
    }

  protected:
    void add_image() {
      Image* image = NULL;

      if (Preferences::getInstance().show_thumbnails()) {
        image = get_image_for_thumbnail();
      }

      if (image == NULL) {
        image = get_image_for_mime_type();
      }
      _set_image(image);
    }

    virtual void add_tooltip() {
      std::string tooltip(file_info->get_display_name());
      tooltip += " - ";
      tooltip += get_file_size_string_from_size(file_info->get_size());
      set_tooltip_text(tooltip);
    }

    virtual void add_markup() {
      if (file_info->get_attribute_boolean (G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE)) {
        bold();
      }
    }

    virtual void connect_signals() {
      signal_button_release_event().connect(sigc::mem_fun(this, &FileItem::on_button_release));
    }

    virtual void on_activate() {
      open_file(path);
    }

    virtual bool on_button_release(const GdkEventButton* event) {
      switch (event->button) {
        case 3:
          on_right_click(event);
          return true;
        case 2:
          TrayIcon::getInstance().popdown();
          on_middle_click();
          return true;
        default:
          return on_left_click();
      }
    }

    virtual bool on_middle_click() {
      return open_file_with_app(Preferences::getInstance().get_alt_file_action(), path);
    }

    IContextMenu* get_context_menu() {
      if (context_menu == NULL)
        context_menu = manage(makeContextMenu(path, *this));
      return context_menu;
    }

    virtual void on_right_click(const GdkEventButton* event) {
      IContextMenu* cm = get_context_menu();
      cm->pop_up(0, event->time);
    }

    virtual bool on_left_click() {
      return false;
    }

  public:
    explicit FileItem(const RefPtr<FileInfo>& file_info,
                      const std::string& path) :
      BaseItem(file_info, path),
      context_menu(NULL) {
    }

    virtual ~FileItem(){
      if (context_menu != NULL) {
        delete(context_menu);
      }
    }
};
/*************************** DesktopItem ***************************************/
class DesktopItem : public FileItem {
  private:
    RefPtr<AppInfo> appinfo;

    Image* get_image_for_desktop_file() {
      Image* image = new Image();
      RefPtr<const Icon> icon = appinfo->get_icon();
      image->set(icon, ICON_SIZE_SMALL_TOOLBAR);
      return image;
    }

  protected:
    virtual void add_image() {
      _set_image(get_image_for_desktop_file());
    }

    virtual void add_markup() {
      bold();
    }

    virtual void connect_signals() {
      signal_button_release_event().connect(sigc::mem_fun(this, &DesktopItem::on_button_release));
    }

    virtual void on_activate() {
      open_file_with_app(appinfo, "");
    }

  public:
    explicit DesktopItem(const Glib::RefPtr<Gio::FileInfo>& file_info,
                         const std::string& path,
                         const Glib::RefPtr<Gio::AppInfo>& appinfo) :
      FileItem(file_info, path),
      appinfo(appinfo) {
      set_label(appinfo->get_name());
    }

    virtual ~DesktopItem(){}
};
/*************************** DirectoryItem *************************************/
class DirectoryItem : public BaseItem {
  private:
    DirectoryListing* listing;

    void add_directory_submenu() {
      listing = manage(new DirectoryListing(file_info, path));
      set_submenu(*listing);
    }

    virtual void on_activate() {
      listing->refresh();
    }

  protected:
    virtual void add_tooltip(){}
    virtual void connect_signals() {}

  public:
    explicit DirectoryItem(const RefPtr<FileInfo>& file_info,
                           const string& path) :
      BaseItem(file_info, path),
      listing(NULL) {

      add_directory_submenu();
    }

    virtual ~DirectoryItem() {
      if (listing != NULL) {
        delete(listing);
      }
    }
};
/*************************** SingleMenuDirectoryItem ************************************/
class SingleMenuDirectoryItem : public FileItem {
  protected:
    virtual void add_markup(){};

    virtual bool on_left_click() {
      DirectoryListing* listing = (DirectoryListing*)get_parent();
      listing->set_path(file_info, path);
      listing->refresh();
      return true;
    }

    virtual void add_tooltip() {
      set_tooltip_text(path);
    }

  public:
    explicit SingleMenuDirectoryItem(const RefPtr<FileInfo>& file_info,
                                     const string& path) :
      FileItem(file_info, path) {
    }

    virtual ~SingleMenuDirectoryItem() {}
};
/*************************** UpDirItem *****************************************/
class UpDirItem : public SingleMenuDirectoryItem {
  public:
    explicit UpDirItem(const RefPtr<File>& directory) :
      SingleMenuDirectoryItem(directory->get_parent()->query_info(),
                              directory->get_parent()->get_path()) {

        // Override the directories display name.
        // TODO: Make this better...
        set_label("..");
    }
};
/*************************** MenuHeaderItem ************************************/
class MenuHeaderItem : public FileItem {
  protected:
    virtual void add_tooltip() {
    }

    virtual void add_markup() {
      bold();
    };

    virtual bool on_middle_click() {
      return open_file_with_app(Preferences::getInstance().get_alt_directory_action(), path);
    }

    void set_tooltip_item_count(int count) {
      std::stringstream tooltip;
      tooltip << file_info->get_display_name();
      tooltip << " - ";
      tooltip << count;
      tooltip << " items";
      set_tooltip_text(tooltip.str());
    }

  public:
    explicit MenuHeaderItem(const RefPtr<FileInfo>& file_info,
                            const string& path,
                            const int children_count) :
      FileItem(file_info, path) {
        set_tooltip_item_count(children_count);
    }

    virtual ~MenuHeaderItem(){}
};
/*************************** Factories *****************************************/
IBaseItem*
makeItem(const RefPtr<FileInfo>& file_info,
         const string& path) {
  BaseItem* item = NULL;

  if (file_is_directory(file_info)) {
      if (Preferences::getInstance().use_single_menu())
        item = new SingleMenuDirectoryItem(file_info, path);
      else
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

IBaseItem*
makeMenuHeaderItem(const RefPtr<FileInfo>& file_info,
                   const string& path,
                   const int children_count) {
  BaseItem* item = new MenuHeaderItem(file_info, path, children_count);
  item->init();
  return item;
}

IBaseItem*
makeUpDirItem(const Glib::RefPtr<Gio::File>& directory) {
  BaseItem* item = new UpDirItem(directory);
  item->init();
  return item;
}
/*******************************************************************************/
} //namespace
