#include <gtkmm.h>
#include <giomm.h>
#include <iostream>
#include "DirectoryItem.h"
#include "PanelMenuBar.h"
#include "DirectoryListing.h"

int main(int argc, char *argv[]) {

  Gtk::Main kit(argc, argv);

  Gtk::Window window;

  Gtk::VBox vbox;
  FileBrowserApplet::PanelMenuBar menu_bar;

  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(Glib::get_home_dir());

  FileBrowserApplet::DirectoryItem item(file->query_info(), file->get_path());
  menu_bar.append(item);

  window.add(vbox);
  vbox.pack_start(menu_bar);

  window.show_all();

  Gtk::Main::run(window);
  return EXIT_SUCCESS;
}
