//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_FLW_HIS_DATA_H__
#define CHAOS_FC_FLW_HIS_DATA_H__
#pragma once

#include "his_head.h"
#include "basic/basictypes.h"
#include <vector>

namespace fc_data {

class FlwHisData {
  friend class FlwHisStk;
 protected:
  FlwHisData();
 public:
  virtual ~FlwHisData();

 protected:
  HIS_DATA_TYPE his_data_type_;
  const HIS_DATA_HEAD* his_data_head_;
 protected:

  //清楚数据
  void ResetData(void);

  //载入数据
  bool LoadData(const unsigned char*& his_buffer);

  //读取数据
  int ReadData(void* buffer, uint32 buffer_size);

};

}

#endif
