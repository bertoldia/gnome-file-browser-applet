#ifndef ITEM_BASE
#define ITEM_BASE

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

class BaseItem : public Gtk::ImageMenuItem {
  protected:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;

  private:
    std::string collate_key;
    void create();
    void add_tooltip();
    void add_markup();
    void add_image();
    Gtk::Image* get_image_for_desktop_file();
    Gtk::Image* get_image_for_thumbnail();
    Gtk::Image* get_image_for_mime_type();
    std::string create_collate_key(const std::string& display_name);

  public:
    BaseItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~BaseItem();
    const std::string& get_collate_key();
};

} //namespace

#endif
