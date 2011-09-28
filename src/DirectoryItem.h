#ifndef DIRECTORY_ITEM
#define DIRECTORY_ITEM

#include <giomm.h>
#include <gtkmm.h>

#include "BaseItem.h"
#include "DirectoryListing.h"

namespace FileBrowserApplet {

class DirectoryItem : public BaseItem {
  private:
    DirectoryListing* listing;

    void add_directory_submenu();
    void on_activate();

  protected:
    explicit DirectoryItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

    virtual void add_image();
    virtual void add_tooltip();
    virtual void connect_signals();

  public:
    static DirectoryItem* make(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
    virtual ~DirectoryItem();
};

} //namespace

#endif
