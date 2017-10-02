//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#include "flw_his_file.h"
#include "stk_datacps.h"
#include "logic/logic_comm.h"
#include "file/file_util.h"
namespace fc_data {

FlwHisFile::FlwHisFile(std::string& base_dir)
    : raw_data_(NULL),
      guid_(NULL),
      file_head_(NULL),
      file_path_(NULL),
      out_base_dir_(base_dir){
}

FlwHisFile::~FlwHisFile() {
    while(his_stk_list_.size()){
        fc_data::FlwHisStk* flw_stk = his_stk_list_.front();
        his_stk_list_.pop_front();
        if(flw_stk){delete flw_stk; flw_stk = NULL;}
    }
}

int FlwHisFile::ProcessHisSTK() {
  while (his_stk_list_.size() > 0) {
    fc_data::FlwHisStk* flw_stk = his_stk_list_.front();
    his_stk_list_.pop_front();
    //printf("symbol %s symbol type %s price_digit %d\n",flw_stk->static_->symbol_,
    //  s_stk_type[flw_stk->static_->ctype_],flw_stk->static_->price_digit_);
    flw_stk->ProcessHisTypeList();
    if(flw_stk) {delete flw_stk; flw_stk = NULL;}
  }

}

int FlwHisFile::OpenHisFile2Read(const std::string& file_path, uint32 trade_date) {
  file_path_ = new file::FilePath(file_path);
  std::string content;
  bool r = file::ReadFileToString(*file_path_, &content);
  raw_data_ = content.c_str();
  guid_ = (const GUID*) (raw_data_);
  bool s = fc_data::IsEqualGUID(*guid_, GUID_FC_HISDATA);
  if (!s) {
    return false;
  }

  file_head_ = (const struct HIS_FILE_HEAD*) (guid_ + 1);
  if (trade_date != 0 && file_head_->market_date_ != trade_date) {
    return false;
  }

  const unsigned char* his_data = (const unsigned char*) file_head_
      + file_head_->size_;
  fc_data::FlwHisStk* last_flw = NULL;
  for (int i = 0; i < file_head_->stk_count_; i++) {
    fc_data::FlwHisStk* flw = new fc_data::FlwHisStk(out_base_dir_);
    if (!flw->LoadStk(his_data,file_head_->market_date_, file_head_->market_)) {
      delete flw;
      return -1;
    }
    his_stk_list_.push_back(flw);
    //his_stk_map_[flw->static_->symbol_] = flw;
    last_flw = flw;
    /*ULOG_DEBUG2("symbol_ %s stk list %d",
     flw->static_->symbol_,
     his_stk_list_.size());*/
  }
  // ULOG_DEBUG2("his_stk_map_ %d",his_stk_map_.size());
  ProcessHisSTK();

  return true;
}

}
