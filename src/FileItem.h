#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "DirectoryListing.h"

class FileItem : public Gtk::ImageMenuItem {
  private:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;
    DirectoryListing* listing;
    bool am_directory;

    void create();
    void add_tooltip();
    void add_markup();
    void add_image();
    void add_directory_submenu();
    void add_signal();
    void on_activate();

  public:
    FileItem(Glib::RefPtr<Gio::FileInfo> file_info, std::string path);
    ~FileItem();

};
#endif
