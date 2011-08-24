#ifndef MENU_HEADER
#define MENU_HEADER

#include "FileItem.h"

namespace FileBrowserApplet {

class MenuHeader : public FileItem {
  private:

  public:
    MenuHeader(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~MenuHeader();
};

} //namespace

#endif
