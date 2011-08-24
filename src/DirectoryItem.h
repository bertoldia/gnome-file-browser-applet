#ifndef DIRECTORY_ITEM
#define DIRECTORY_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "ItemBase.h"

namespace FileBrowserApplet {

class DirectoryItem : public ItemBase {
  protected:
    DirectoryListing* listing;
    sigc::connection _signal_activate;

    void create();
    void add_directory_submenu();
    void on_activate();
    std::string create_collate_key(const std::string& display_name);

  public:
    DirectoryItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~DirectoryItem();
};

}

#endif
