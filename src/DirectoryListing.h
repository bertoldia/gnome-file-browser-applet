#ifndef DIRECTORY_LISTING
#define DIRECTORY_LISTING

#include <gtkmm.h>

class DirectoryListing : public Gtk::Menu {
  private:
    std::string path;
    Gtk::MenuItem* item;

    void clear();
    void populate();
    void create();
    void add_header();
    void add_directories();
    void add_files();

  public:
    DirectoryListing(std::string path);
    ~DirectoryListing();
    void refresh();
};
#endif
