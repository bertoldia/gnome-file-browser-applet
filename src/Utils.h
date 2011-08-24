#ifndef UTILS
#define UTILS

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

bool file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info);
std::string get_file_size_string_from_size(long size);

}

#endif

