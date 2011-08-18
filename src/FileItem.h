#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "DirectoryListing.h"

namespace FileBrowserApplet {

class FileItem : public Gtk::ImageMenuItem {
  protected:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;
    std::string collate_key;
    DirectoryListing* listing;
    bool am_directory;
    sigc::connection _signal_activate;

    void create();
    void add_tooltip();
    void add_markup();
    void add_image();
    void add_directory_submenu();
    void on_activate();
    std::string create_collate_key(const std::string& display_name);

  public:
    FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~FileItem();
    bool is_directory();
    const std::string& get_collate_key();

};

}

#endif
