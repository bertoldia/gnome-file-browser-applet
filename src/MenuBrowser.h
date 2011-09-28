#ifndef MENU_BROWSER
#define MENU_BROWSER

#include <glibmm.h>
#include <giomm.h>

#include "DirectoryItem.h"

namespace FileBrowserApplet {

class MenuBrowser : public DirectoryItem {
  private:

    explicit MenuBrowser(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path, const std::string& label);

  public:
    static MenuBrowser* make(const std::string& path, const std::string& label);

    void set_label(std::string& label);
    void set_path(std::string& path);
    void set_show_icon(bool show);
    //void set_icon(??);
};

}

#endif

