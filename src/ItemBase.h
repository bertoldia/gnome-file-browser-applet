#ifndef ITEM_BASE
#define ITEM_BASE

#include <giomm.h>
#include <gtkmm.h>
#include "DirectoryListing.h"

namespace FileBrowserApplet {

class ItemBase : public Gtk::ImageMenuItem {
  protected:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;

  private:
    std::string collate_key;
    void create();
    void add_tooltip();
    void add_markup();
    void add_image();
    std::string create_collate_key(const std::string& display_name);

  public:
    ItemBase(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~ItemBase();
    const std::string& get_collate_key();
};

}

#endif

