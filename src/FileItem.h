#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"

namespace FileBrowserApplet {

class FileItem : public BaseItem {
  protected:
    void create();

  public:
    FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    ~FileItem();
};

} //namespace

#endif
