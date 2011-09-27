#include "PanelMenuBar.h"

namespace FileBrowserApplet {

// initialize static member
PanelMenuBar* PanelMenuBar::instance(NULL);

PanelMenuBar::PanelMenuBar() {
}

PanelMenuBar&
PanelMenuBar::getInstance() {
  if (PanelMenuBar::instance == NULL) {
    PanelMenuBar::instance = new PanelMenuBar();
  }
  return *(PanelMenuBar::instance);
}

}
