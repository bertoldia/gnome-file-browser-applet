#include "PanelMenuBar.h"
#include "Items.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;

// initialize static member
PanelMenuBar* PanelMenuBar::instance(NULL);

PanelMenuBar::PanelMenuBar() {
  add_browser(get_home_dir(), "Home");
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
  IBaseItem* browser = manage(makeMenuBrowser(path, label));
  append(*browser);
}

}
