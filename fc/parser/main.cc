//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.29 Author: kerry

#include "file/file_path.h"
#include "file/file_util.h"
#include "logic/logic_comm.h"
#include "flw_his_file.h"
#include <string>

const char* me = NULL;

void usage() {
    LOG_MSG("-dir {path} {out_base_dir}");
    LOG_MSG("-file {filename} {out_base_dir}");
}

int main(int agrc, char* argv[]) {
  me = argv[0];
  LOG_MSG2("%s", me);
  if (agrc < 3) {
    usage();
    return 1;
  }
  LOG_MSG2("%s",argv[1]);
  if (argv[1][0] == '-' && argv[1][1] == 'd' && argv[1][2] == 'i'
      && argv[1][3] == 'r') {
    std::string dir = argv[2];
    std::string out_dir = argv[3];
    LOG_MSG2("%s", dir.c_str());
    file::FilePath dir_path(dir);
    std::list<file::FilePath> file_list;
    file::GetDirectoryFile(dir_path, file_list);
    while (file_list.size() > 0) {
      file::FilePath file = file_list.front();
      file_list.pop_front();
      LOG_MSG2("file:%s", file.value().c_str());
      fc_data::FlwHisFile his_file(out_dir);
      his_file.OpenHisFile2Read(file.value());
    }
  }else if(argv[1][0] == '-' && argv[1][1] == 'f' && argv[1][2] == 'i'
          && argv[1][3] == 'l' && argv[1][4] == 'e'){
      std::string file = argv[2];
      std::string out_dir = argv[3];
      fc_data::FlwHisFile his_file(out_dir);
      his_file.OpenHisFile2Read(file);
   }else{
    usage();
  }
}
