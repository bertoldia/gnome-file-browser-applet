#include "MenuBrowser.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;
using namespace Gio;
using namespace Gtk;

MenuBrowser*
MenuBrowser::make(const string& path, const std::string& label) {
  RefPtr<File> file = File::create_for_path(path);
  MenuBrowser* item = new MenuBrowser(file->query_info(), path, label);
  item->init();
  return item;
}

MenuBrowser::MenuBrowser(const RefPtr<FileInfo>& file_info, const string& path, const string& label) :
  DirectoryItem(file_info, path) {
  set_label(label);
}

  //select_first();

}

