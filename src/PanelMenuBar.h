#ifndef PANEL_MENU_BAR
#define PANEL_MENU_BAR

#include <gtkmm.h>

namespace FileBrowserApplet {

class PanelMenuBar : public Gtk::MenuBar {
  private:
    static PanelMenuBar* instance;

    void add_browser(const std::string& path, const std::string& label);

    explicit PanelMenuBar();

  public:
    static PanelMenuBar& getInstance();
};

}

#endif
