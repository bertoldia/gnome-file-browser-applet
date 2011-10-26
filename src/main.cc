#include <gtkmm.h>
#include <giomm.h>
#include <iostream>
#include "PanelMenuBar.h"
#include "PanelMenuBar.h"
#include "TrayIcon.h"

using namespace FileBrowserApplet;

int main(int argc, char* argv[]) {

  Gtk::Main kit(argc, argv);

  //Gtk::Window window;

  //Gtk::VBox vbox;
  //PanelMenuBar& menu_bar(PanelMenuBar::getInstance());

  //window.add(vbox);
  //vbox.pack_start(menu_bar);

  //window.show_all();


  TrayIcon* tray = new TrayIcon();
  //Gtk::Main::run(window);
  Gtk::Main::run();
  return EXIT_SUCCESS;
}
