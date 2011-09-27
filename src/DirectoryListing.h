#ifndef DIRECTORY_LISTING
#define DIRECTORY_LISTING

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"
#include "MenuHeader.h"

namespace FileBrowserApplet {

class DirectoryListing : public Gtk::Menu {
  private:
    std::string path;
    MenuHeader* header;

    void clear();
    void populate();
    void query_file_system_sync();
    void add_children_entries(const Glib::RefPtr<Gio::FileEnumerator>& children);
    void add_header(const Glib::RefPtr<Gio::FileInfo>& file_info);
    void add_separator();
    void add_empty_item();
    void add_items(std::vector<BaseItem*> items);

  public:
    explicit DirectoryListing(const std::string& path);
    ~DirectoryListing();
    void refresh(const Glib::RefPtr<Gio::FileInfo>& file_info);
};

} //namespace

#endif
