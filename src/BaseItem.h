#ifndef ITEM_BASE
#define ITEM_BASE

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

class BaseItem : public Gtk::ImageMenuItem {
  protected:
    Glib::RefPtr<Gio::FileInfo> file_info;
    std::string path;

    virtual void add_image(){};
    virtual void add_tooltip(){};
    virtual void add_markup(){};
    virtual void connect_signals(){};

    Gtk::Image* get_image_for_mime_type();
    void _set_image(Gtk::Image* image);
    void bold();

    void init();

  private:
    std::string collate_key;
    std::string create_collate_key(const std::string& display_name);
    std::string get_display_name_string(std::string display_name);
    void set_ellipsize();

  public:
    explicit BaseItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~BaseItem();
    const std::string& get_collate_key();
};

} //namespace

#endif
