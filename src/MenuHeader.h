#ifndef MENU_HEADER
#define MENU_HEADER

#include "FileItem.h"

namespace FileBrowserApplet {

class MenuHeader : public FileItem {
  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void add_markup(){};

    virtual bool on_middle_click();

    explicit MenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

  public:
    static MenuHeader* make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~MenuHeader();
};

} //namespace

#endif
