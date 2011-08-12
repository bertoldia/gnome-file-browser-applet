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
    void add_children_entries(const Glib::RefPtr<Gio::FileEnumerator>& children);
    void add_header(const Glib::RefPtr<Gio::FileInfo>& file_info);
    void add_directories();
    void add_files();
    void add_separator();

  public:
    explicit DirectoryListing(const std::string& path);
    ~DirectoryListing();
    void refresh(const Glib::RefPtr<Gio::FileInfo>& file_info);
};
#endif
