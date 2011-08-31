#include "Utils.h"
#include "FileItem.h"
#include "DirectoryItem.h"

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

BaseItem* make_item(const Glib::RefPtr<Gio::FileInfo>& file_info, const std::string& parent) {
  if (file_is_directory(file_info)) {
    return DirectoryItem::make(file_info, parent + "/" + file_info->get_name());
  } else {
    return FileItem::make(file_info, parent + "/" + file_info->get_name());
  }
}

} //namespace
