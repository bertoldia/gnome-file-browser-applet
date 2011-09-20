#ifndef DESKTOP_ITEM
#define DESKTOP_ITEM

#include <giomm.h>
#include <gdkmm.h>
#include <gtkmm.h>
#include "FileItem.h"

namespace FileBrowserApplet {

class DesktopItem : public FileItem {
  private:
    Glib::RefPtr<Gio::DesktopAppInfo> appinfo;

    Gtk::Image* get_image_for_desktop_file();

  protected:
    virtual void add_image();
    virtual void add_markup();
    virtual void connect_signals();

    void on_activate_desktop_item();
    bool on_button_release(GdkEventButton* event);

    explicit DesktopItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

  public:
    static DesktopItem* make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~DesktopItem();
};

} //namespace

#endif
