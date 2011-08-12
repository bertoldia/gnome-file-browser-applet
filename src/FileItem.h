#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "DirectoryListing.h"

class FileItem : public Gtk::ImageMenuItem {
  protected:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;
    DirectoryListing* listing;
    bool am_directory;
    sigc::connection _signal_activate;

    void create();
    void add_tooltip();
    void add_markup();
    void add_image();
    void add_directory_submenu();
    void on_activate();

  public:
    FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~FileItem();

};
#endif
