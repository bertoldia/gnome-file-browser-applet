#include <glibmm.h>
#include "Preferences.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Glib;

// initialize static member
Preferences* Preferences::instance(NULL);

Preferences::Preferences():
  _show_hidden(false) {
  // load prefs
  // load ui file
}

Preferences&
Preferences::getInstance() {
  if (Preferences::instance == NULL) {
    Preferences::instance = new Preferences();
  }
  return *(Preferences::instance);
}

bool
Preferences::show_hidden() {
  return _show_hidden;
}

void
Preferences::show_hidden(bool hidden) {
  _show_hidden = hidden;
}

bool
Preferences::show_dirs_only() {
  return false;
}

bool
Preferences::show_thumbnails() {
  return true;
}

bool
Preferences::confirm_exec_action() {
  return true;
}

bool
Preferences::show_tooltips() {
  return true;
}

bool
Preferences::return_home_on_close() {
  bool return_home_on_close = false;
  return return_home_on_close && use_single_menu();
}

bool
Preferences::use_single_menu() {
  return true;
}

string
Preferences::get_alt_file_action() {
  return "gvim";
}

string
Preferences::get_alt_directory_action() {
  return "gnome-terminal";
}

string
Preferences::get_root_directory() {
  return get_home_dir();
}

}
