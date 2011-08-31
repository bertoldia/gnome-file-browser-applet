#ifndef FILE_ITEM
#define FILE_ITEM

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"

namespace FileBrowserApplet {

class FileItem : public BaseItem {
  protected:
    virtual void add_image();
    virtual void add_tooltip();
    virtual void add_markup();
    virtual void connect_signals();

    explicit FileItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

  public:
    static FileItem* make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~FileItem();
};

} //namespace

#endif
