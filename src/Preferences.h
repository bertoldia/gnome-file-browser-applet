#ifndef PREFERENCES
#define PREFERENCES

#include <iostream>

class Preferences {
  private:
    static Preferences* instance;

    explicit Preferences();

  public:
    static Preferences* getInstance();

    bool show_hidden();
    bool show_dirs_only();
    bool show_thumbnails();
    bool confirm_exec_action();
};

#endif
