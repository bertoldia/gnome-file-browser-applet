#ifndef UTILS
#define UTILS

#include <giomm.h>
#include <gtkmm.h>

namespace FileBrowserApplet {

bool file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info);
std::string get_file_size_string_from_size(long size);
bool open_file(const std::string& path);
bool open_file_with_app(const std::string& app, const std::string& path);
bool open_file_with_app(const Glib::RefPtr<Gio::AppInfo>& appinfo, const std::string& path);
void show_dialog(std::string title, std::string message, Gtk::StockID image);

} //namespace

#endif
