#include "PanelMenuBar.h"
#include "MenuBrowser.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;

// initialize static member
PanelMenuBar* PanelMenuBar::instance(NULL);

PanelMenuBar::PanelMenuBar() {
  add_browser(get_home_dir(), "");
}

PanelMenuBar&
PanelMenuBar::getInstance() {
  if (PanelMenuBar::instance == NULL) {
    PanelMenuBar::instance = new PanelMenuBar();
  }
  return *(PanelMenuBar::instance);
}

void
PanelMenuBar::add_browser(const string& path, const string& label) {
  MenuBrowser* browser = manage(MenuBrowser::make(path, label));
  append(*browser);
}

}
