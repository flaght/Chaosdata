//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_FLW_HIS_FILE_H__
#define CHAOS_FC_FLW_HIS_FILE_H__
#pragma once
#include "flw_his_stk.h"
#include "file/file_path.h"
#include "his_head.h"
#include <map>
#include <list>
#include <string>

namespace fc_data {

typedef std::map<std::string, fc_data::FlwHisStk*> HISSTK_MAP;
typedef std::list<fc_data::FlwHisStk*> HISSTK_LIST;
class FlwHisFile {
 public:
  FlwHisFile(std::string& base_dir);
  virtual ~FlwHisFile();
 private:
  const char* raw_data_;
  const GUID* guid_;
  const HIS_FILE_HEAD* file_head_;
  file::FilePath* file_path_;
 private:
  HISSTK_MAP his_stk_map_;
  HISSTK_LIST his_stk_list_;
  std::string out_base_dir_;
 public:
  //打开历史数据文件，校验是否指定交易日
  virtual int OpenHisFile2Read(const std::string& file_path, uint32 trade_date = 0);  //交易日，0表示不限定交易日


  //遍历文件内容
  virtual int ProcessHisSTK();  //遍历解析后文件的内容
};

}

#endif
