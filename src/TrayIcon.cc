 /*
 * Copyright 2011 Axel von Bertoldi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to:
 * The Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor
 * Boston, MA 02110-1301, USA.
 */

#include <giomm.h>
#include "TrayIcon.h"

#ifdef LIBGTKHOTKEY_FOUND
#include <gtkhotkey.h>
#endif

namespace FileBrowserApplet {

using namespace std;
using namespace Gio;
using namespace Glib;
using namespace Gtk;

// initialize static member
TrayIcon* TrayIcon::instance(NULL);


int
TrayIcon::main(int argc, char** argv) {
  Gtk::Main::run();
  return EXIT_SUCCESS;
}

TrayIcon&
TrayIcon::getInstance() {
  if (instance == NULL) {
    instance = new TrayIcon();
  }
  return *instance;
}


TrayIcon::TrayIcon() {
  set("user-home");
  set_title("Home");
  set_tooltip_text("Browser your home folder");
  set_has_tooltip(true);

  init_browser_menu();
  init_meta_menu();

  signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_tray_icon_activate));
  signal_popup_menu().connect(sigc::mem_fun(this, &TrayIcon::on_popup_menu));

#ifdef LIBGTKHOTKEY_FOUND
  connect_hotkey();
#endif
}

TrayIcon::~TrayIcon() {
}

// No C++ bindings for libgtkhotkey.
#ifdef LIBGTKHOTKEY_FOUND
void
TrayIcon::connect_hotkey() {
    GError *error = NULL;
    GtkHotkeyInfo *hot_key_info = gtk_hotkey_info_new ("fba",
                                                       "fba",
                                                       "<Shift><Ctrl>h",
                                                       NULL);
    g_signal_connect (G_OBJECT (hot_key_info),
                      "activated",
                      G_CALLBACK (&TrayIcon::on_hotkey_pressed),
                      NULL);
    gtk_hotkey_info_bind (hot_key_info, &error);
}

void
TrayIcon::on_hotkey_pressed() {
  getInstance().on_tray_icon_activate();
}
#endif

void
TrayIcon::on_tray_icon_activate() {
  browser_menu->refresh();
  popup_menu_at_position(*browser_menu, 0, gtk_get_current_event_time());
  browser_menu->select_first();
}

void
TrayIcon::on_popup_menu(guint button, guint32 activate_time) {
  popup_menu_at_position(*meta_menu, 0, gtk_get_current_event_time());
}

void
TrayIcon::init_browser_menu() {
  string path = get_home_dir();
  browser_menu = manage(new DirectoryListing(File::create_for_path(path)->query_info(), path));
}

void
TrayIcon::init_meta_menu() {
  meta_menu = manage(new Menu());

  ImageMenuItem* item;

  item = manage(new ImageMenuItem(Stock::PREFERENCES));
  meta_menu->append(*item);

  item = manage(new ImageMenuItem(Stock::ABOUT));
  meta_menu->append(*item);
  item->signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_about));

  item = manage(new ImageMenuItem(Stock::QUIT));
  meta_menu->append(*item);
  item->signal_activate().connect(sigc::mem_fun(this, &TrayIcon::on_quit));

  meta_menu->show_all();
}

void
TrayIcon::on_quit() {
  Gtk::Main::quit();
}

void
TrayIcon::popdown() {
  browser_menu->popdown();
}

void
TrayIcon::on_about() {
  std::vector<Glib::ustring> authors;
  authors.push_back("Axel von Bertoldi <bertoldia@gmail.com>");
  authors.push_back("Contributions by:");
  authors.push_back("Ivan N. Zlatev <contact@i-nz.net>");
  authors.push_back("Stefano Maggiolo <maggiolo@mail.dm.unipi.it>");
  authors.push_back("Deji Akingunola <dakingun@gmail.com>");
  authors.push_back("Serkan Kaba <serkan@gentoo.org>");
  authors.push_back("Silvio Ricardo Cordeiro <silvioricardoc@gmail.com>");
  authors.push_back("pachoramos");
  authors.push_back("Ivan Baldo <ibaldo@adinet.com.uy>");

  std::vector<Glib::ustring> documenters;
  documenters.push_back("You!!! That's right! You can help!");

  std::string translators("translator-credits");
  if (translators == "translator-credits")
    translators = "You!!! That's right! You can help!";

  AboutDialog about;
  about.set_name("File Browser Applet");
  about.set_program_name("File Browser Applet");
  about.set_version("1.0");
  about.set_logo(IconTheme::get_default()->load_icon("user-home", 48, (Gtk::IconLookupFlags) 0));
  about.set_copyright("Copyright \xc2\xa9 2006-2011 Axel von Bertoldi");
  about.set_comments("Browse and open files in your home directory from the panel.");

  about.set_website("http://code.google.com/p/gnome-menu-file-browser-applet/");
  about.set_website_label("Homepage");
  about.set_authors(authors);
  about.set_documenters(documenters);
  about.set_translator_credits(translators);
  about.set_icon_name("user-home");
  about.run();
}

} //namespace
