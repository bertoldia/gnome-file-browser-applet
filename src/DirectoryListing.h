#ifndef DIRECTORY_LISTING
#define DIRECTORY_LISTING

#include <gtkmm.h>
#include <giomm.h>

class DirectoryListing : public Gtk::Menu {
  private:
    std::string path;
    Gtk::MenuItem* _item;

    void clear();
    void populate();
    void query_file_system_sync();
    void add_children_entries(Glib::RefPtr<Gio::FileEnumerator> children);
    void add_header(Glib::RefPtr<Gio::FileInfo>& file_info);
    void add_directories();
    void add_files();
    void add_separator();

  public:
    DirectoryListing(std::string path);
    ~DirectoryListing();
    void refresh(Glib::RefPtr<Gio::FileInfo>& file_info);
};
#endif
