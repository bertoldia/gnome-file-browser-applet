#ifndef DIRECTORY_ITEM
#define DIRECTORY_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"
#include "DirectoryListing.h"

namespace FileBrowserApplet {

class DirectoryItem : public BaseItem {
  protected:
    DirectoryListing* listing;
    sigc::connection _signal_activate;

    void create();
    void add_directory_submenu();
    void on_activate();

  public:
    DirectoryItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~DirectoryItem();
};

}

#endif
