#ifndef PREFERENCES
#define PREFERENCES

#include <iostream>

namespace FileBrowserApplet {

class Preferences {
  private:
    static Preferences* instance;

    explicit Preferences();

  public:
    static Preferences& getInstance();

    bool show_hidden();
    bool show_dirs_only();
    bool show_thumbnails();
    bool confirm_exec_action();
    bool show_tooltips();
};

}

#endif