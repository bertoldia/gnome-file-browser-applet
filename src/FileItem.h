#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gdkmm.h>
#include <gtkmm.h>
#include "BaseItem.h"

namespace FileBrowserApplet {

class FileItem : public BaseItem {
  private:
    bool is_desktop;

    Gtk::Image* get_image_for_desktop_file();
    Gtk::Image* get_image_for_thumbnail();

  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void add_markup();
    virtual void connect_signals();

    void on_activate_file_item();
    bool on_button_release(GdkEventButton* event);

    explicit FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

  public:
    static FileItem* make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~FileItem();
};

} //namespace

#endif
