//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#include "his_head.h"
#include "flw_his_data.h"
#include "logic/logic_comm.h"
namespace fc_data {

FlwHisData::FlwHisData()
    : his_data_type_(HIS_DATA_TYPE_UNKNOWN),
      his_data_head_(NULL) {

}

FlwHisData::~FlwHisData() {
  ResetData();
}

//清除数据
void FlwHisData::ResetData() {
  his_data_type_ = HIS_DATA_TYPE_UNKNOWN;
  his_data_head_ = NULL;
}

//载入数据
bool FlwHisData::LoadData(const unsigned char*& his_buffer) {
  ResetData();

  his_data_type_ = *((HIS_DATA_TYPE*) his_buffer);
  his_buffer = his_buffer + sizeof(HIS_DATA_TYPE);

  his_data_head_ = (const struct HIS_DATA_HEAD*) his_buffer;
  his_buffer = his_buffer + his_data_head_->head_size_
      + his_data_head_->package_size_;

  /*printf("head_size:%d  item_size:%d item_count:%d package_flag:%d package_size:%d\n",
   his_data_head_->head_size_, his_data_head_->item_size_, his_data_head_->item_count_,
   his_data_head_->package_flag_, his_data_head_->package_size_);*/
  return true;
}

//读取数据
int FlwHisData::ReadData(void* buffer, uint32 buffer_size) {
  if (NULL == his_data_head_)
    return -1;

  uint32 data_size = his_data_head_->head_size_ + his_data_head_->package_size_;

  if (buffer_size < data_size)
    return data_size;

  if (NULL == buffer)
    return -1;
  memcpy(buffer, his_data_head_, data_size);
  return data_size;
}

}
