#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <iostream>

namespace FileBrowserApplet {

class Preferences {
  private:
    static Preferences* instance;
    bool _show_hidden;

    explicit Preferences();

  public:
    static Preferences& getInstance();

    bool show_hidden();
    void show_hidden(bool hidden);
    bool show_dirs_only();
    bool show_thumbnails();
    bool confirm_exec_action();
    bool show_tooltips();
    bool return_home_on_close(); // only relevant if use_single_menu is true.
    bool use_single_menu();
    std::string get_alt_file_action();
    std::string get_alt_directory_action();
};

}

#endif
