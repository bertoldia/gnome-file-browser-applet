#ifndef UTILS
#define UTILS

#include <giomm.h>
#include <gtkmm.h>
#include "BaseItem.h"

namespace FileBrowserApplet {

bool file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info);
std::string get_file_size_string_from_size(long size);
bool open_file(const std::string& path);
bool open_file_with(const std::string& app, const std::string& path);
BaseItem* makeItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path);
bool launch_desktop_file(const Glib::RefPtr<Gio::AppInfo>& appinfo, const std::string& path);

} //namespace

#endif
