#ifndef UTILS
#define UTILS

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"

namespace FileBrowserApplet {

bool file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info);
std::string get_file_size_string_from_size(long size);
BaseItem* make_item(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);

}

#endif

