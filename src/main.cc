#include <gtkmm.h>
#include <giomm.h>
#include <iostream>
#include "PanelMenuBar.h"

using namespace FileBrowserApplet;

int main(int argc, char* argv[]) {

  Gtk::Main kit(argc, argv);

  Gtk::Window window;

  Gtk::VBox vbox;
  PanelMenuBar& menu_bar(PanelMenuBar::getInstance());

  window.add(vbox);
  vbox.pack_start(menu_bar);

  window.show_all();

  Gtk::Main::run(window);
  return EXIT_SUCCESS;
}
