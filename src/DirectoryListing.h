#ifndef DIRECTORY_LISTING
#define DIRECTORY_LISTING

#include <giomm.h>
#include <gtkmm.h>

#include "Items.h"

namespace FileBrowserApplet {

class DirectoryListing : public Gtk::Menu {
  private:
    std::string path;
    Glib::RefPtr<Gio::FileInfo> file_info;

    void clear();
    void remove_child(Gtk::Widget& widget);
    void populate();
    void query_file_system_sync();
    void add_children_entries(const Glib::RefPtr<Gio::FileEnumerator>& children);
    void add_header(const unsigned int children_count);
    void add_separator();
    void add_error_item(const std::string& message);
    void add_items(std::vector<IBaseItem*> items);
    void show_limited();
    void add_more_item();
    bool on_button_release_more_item(const GdkEventButton* event);
    bool on_key_release_more_item(const GdkEventKey* event);
    bool on_activate_more_item();

  public:
    explicit DirectoryListing(const Glib::RefPtr<Gio::FileInfo>& file_info,
                              const std::string& path);
    ~DirectoryListing();
    void refresh();
};

} //namespace

#endif
