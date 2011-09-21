#include "Utils.h"
#include <iostream>
#include <glib/gstdio.h>

#include "DirectoryItem.h"
#include "FileItem.h"
#include "DesktopItem.h"

namespace FileBrowserApplet {

const std::string FILE_SIZE_UNITS[] = {"bytes","KB","MB","GB","TB","HUGE"};
const int FILE_SIZE_ORDER_OF_MAGNITUDE = 1024;

bool
file_is_directory(const Glib::RefPtr<Gio::FileInfo>& file_info) {
  return Gio::FILE_TYPE_DIRECTORY == file_info->get_file_type();
}

std::string
get_file_size_string_from_size(long size) {
  double _size(size);

  int order_of_magnitude = 0;
  while (_size > FILE_SIZE_ORDER_OF_MAGNITUDE){
      _size = _size / FILE_SIZE_ORDER_OF_MAGNITUDE;
      order_of_magnitude++;
  }

  std::stringstream result;
  result.precision(1);
  result << std::fixed;
  result << _size;
  result << " ";
  result << (order_of_magnitude <= 5 ?  FILE_SIZE_UNITS[order_of_magnitude] : FILE_SIZE_UNITS[5]);

  return result.str();
}

bool
open_file(const std::string& path) {
  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
  if(!file->query_exists()) {
    // FIXME: do something!
    return false;
  }

  std::cout << "opening " << file->get_uri() << std::endl;
  g_chdir(file->get_path().c_str());
  Gio::AppInfo::launch_default_for_uri(file->get_uri());
  g_chdir(Glib::get_home_dir().c_str());
  return false;
}

BaseItem*
makeItem(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& path) {
  if (file_is_directory(file_info)) {
    return DirectoryItem::make(file_info, path);
  } else {
    Glib::RefPtr<Gio::AppInfo> appinfo = Gio::DesktopAppInfo::create_from_filename(path);
    if (appinfo) {
      return DesktopItem::make(file_info, path, appinfo);
    } else {
      return FileItem::make(file_info, path);
    }
  }
}

bool
launch_desktop_file(const std::string& path) {
  return true;
}

} //namespace
