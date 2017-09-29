//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.29 Author: kerry

#include "file/file_path.h"
#include "file/file_util.h"
#include <string>

const char* me = NULL;

void usage() {

}

int main(int agrc, char* argv[]) {
  me = argv[0];
  ULOG_MSG2("%s", me);
  if (args < 2) {
    usage();
    return 1;
  }
  if (argv[1][0] == '-' && argv[1][1] == 'd' && argv[1][2] == 'i'
      && argv[1][3] == 'r') {
    std::string dir = argv[2];
    file::FilePath dir_path(dir);
    std::list<file::FilePath> file_list;
    file::GetDirectoryFile(dir_path, file_list);
    while (file_list.size() > 0) {
      file::FilePath file = file_list.front();
      file_list.pop_front();
      ULOG_DEBUG2("%s", file.value().c_str());
    }
  }
}
