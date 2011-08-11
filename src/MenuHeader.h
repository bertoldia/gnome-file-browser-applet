#ifndef MENU_HEADER
#define MENU_HEADER

#include "FileItem.h"

class MenuHeader : public FileItem {
  private:
    virtual void add_directory_submenu();

  public:
    MenuHeader(Glib::RefPtr<Gio::FileInfo> file_info, std::string path);
    ~MenuHeader();

};
#endif
