#include "Preferences.h"

namespace FileBrowserApplet {

// initialize static member
Preferences* Preferences::instance(NULL);

Preferences::Preferences() {
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
  return false;
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

std::string
Preferences::get_alt_file_action() {
  return "gvim";
}

std::string
Preferences::get_alt_directory_action() {
  return "gnome-terminal";
}

}
