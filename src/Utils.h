#ifndef UTILS
#define UTILS

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

bool file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info);

}

#endif

