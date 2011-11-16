#include <vector>
#include <iostream>
#include <glib/gstdio.h>

#include "Utils.h"

namespace FileBrowserApplet {

using namespace std;
using namespace Gio;
using namespace Glib;
using namespace Gtk;

const string FILE_SIZE_UNITS[] = {"bytes","KB","MB","GB","TB","HUGE"};
const int FILE_SIZE_ORDER_OF_MAGNITUDE = 1024;

bool
file_is_directory(const RefPtr<FileInfo>& file_info) {
  return FILE_TYPE_DIRECTORY == file_info->get_file_type();
}

string
get_file_size_string_from_size(long size) {
  double _size(size);

  int order_of_magnitude = 0;
  while (_size > FILE_SIZE_ORDER_OF_MAGNITUDE){
      _size = _size / FILE_SIZE_ORDER_OF_MAGNITUDE;
      order_of_magnitude++;
  }

  stringstream result;
  result.precision(1);
  result << fixed;
  result << _size;
  result << " ";
  result << (order_of_magnitude <= 5 ?  FILE_SIZE_UNITS[order_of_magnitude] : FILE_SIZE_UNITS[5]);

  return result.str();
}

bool
open_file(const string& path) {
  RefPtr<File> file = File::create_for_path(path);
  if(!file->query_exists()) {
    show_dialog("Error", "Failed to open file: " + path + " does not exists.",
                Stock::DIALOG_ERROR);
    return false;
  }

  cout << "opening " << file->get_uri() << endl;
  g_chdir(file->get_path().c_str());
  try {
    AppInfo::launch_default_for_uri(file->get_uri());
  } catch (Glib::Error e) {
    show_dialog("Error", e.what(), Stock::DIALOG_ERROR);
  }

  g_chdir(get_home_dir().c_str());
  return false;
}

bool
open_file_with_app(const string& app, const string& path) {
  RefPtr<AppInfo> appinfo = AppInfo::create_from_commandline(app, "", APP_INFO_CREATE_NONE);
  return open_file_with_app(appinfo, path);
}

bool
open_file_with_app(const RefPtr<AppInfo>& appinfo, const string& path) {
  if (!appinfo) {
    show_dialog("Error", "Could not open " + path +
                " with the specified application.", Stock::DIALOG_ERROR);
    return false;
  }

  vector<RefPtr<File> > files;
  if (!path.empty()) {
    files.push_back(File::create_for_path(path));
  }

  RefPtr<AppLaunchContext> launch_context(0);
  g_chdir(path.c_str());
  bool ret = false;
  try {
    ret = appinfo->launch(files, launch_context);
  } catch (Glib::Error e) {
    show_dialog("Error", e.what(), Stock::DIALOG_ERROR);
  }
  g_chdir(get_home_dir().c_str());
  return ret;
}

void
show_dialog(std::string title, std::string message, Gtk::StockID image) {
  MessageDialog dialog("<span weight=\"bold\">" + message + "</span>", true);
  dialog.set_title(title);
  dialog.set_image((Widget&)*(new Image(image, ICON_SIZE_DIALOG)));
  dialog.show_all();
  dialog.run();
}

} //namespace
