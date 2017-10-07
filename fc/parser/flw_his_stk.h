//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_FLW_HIS_STK_H__
#define CHAOS_FC_FLW_HIS_STK_H__
#pragma once

#include "flw_his_data.h"
#include "his_head.h"
#include "proto/symbol_dynam_market.pb.h"
#include "proto/symbol_static.pb.h"
#include "proto/symbol_l2_mmpex.pb.h"
#include "proto/symbol_l2_order_state.pb.h"
#include "proto/symbol_l2_report.pb.h"
#include "proto/symbol_his_iopv.pb.h"
#include "proto/symbol_his_matu_yld.pb.h"
#include "proto/symbol_pos_index.pb.h"
#include "basic/basictypes.h"
#include <string>
#include <vector>
#include <map>
#include <list>
namespace fc_data {

typedef std::map<int32, fc_data::FlwHisData*> HISDATA_MAP;
typedef std::list<fc_data::FlwHisData*> HISDATA_LIST;

extern const chaos_data::SymbolStatic_SYMBOL_TYPE g_gpb_data_type[chaos_data::SymbolStatic_SYMBOL_TYPE_SYMBOL_TYPE_ARRAYSIZE];

class FlwHisStk {
  friend class FlwHisFile;
 protected:
  FlwHisStk(std::string& dir);
 public:
  virtual ~FlwHisStk();
 private:
  HISDATA_LIST his_data_list_;
  HISDATA_MAP his_data_map_;  //按照数据类型存储
 protected:
  const int32* static_size_;
  const struct STK_STATIC* static_;
  const int32* staticex_size_;
  const struct STK_STATICEx* static_ex_;
  const int32* his_data_count_;

 private:
  chaos_data::SymbolPosIndex last_pos_index_;
 private:
  std::string data_buffer_;
  struct HIS_DATA_HEAD* data_head_;
  void* data_ptr_;
  uint32 market_date_;
  char market_mtk_[32];
  std::string out_dir_;

 protected:
  //清楚数据
  void RestStk(void);

  //载入数据
  bool LoadStk(const unsigned char* & his_buffer, const uint32 market_date,
               const uint16 market_mtk);

  //获取证券代码
  bool GetStkStatic(STK_STATIC& stk_code);

  //读取证券数据类型列表
  bool GetDataTypeList(std::vector<int>& list);

  //遍历对应的数据类型
  bool ProcessHisTypeList();

  //遍历对应类型数据
  void ProcessTypeData(fc_data::FlwHisData* flw_data);

  //写入静态数据类型
  void WriteStatic(HIS_DATA_TYPE data_type, const int32 year, const int32 month,
                   const int32 day);

  //google protobuf 写入
  void GoogleProtobufWriteFile(const char* symbol, HIS_DATA_TYPE data_type,
                               const int32 year, const int32 month,
                               const int32 day, const int32 hour,
                               const void* data_ptr);

  //设置索引
  void SetIndexPos(const int32 tt_time, const int32 packet_length,
                   HIS_DATA_TYPE data_type);

 private:
  void WriteDynaData(HIS_DATA_TYPE data_type);
  void WriteL2MMPEX(HIS_DATA_TYPE data_type);
  void WriteL2Report(HIS_DATA_TYPE data_type);
  void WriteOrderStat(HIS_DATA_TYPE data_type);
  void WriteIOPV(HIS_DATA_TYPE data_type);
  void WriteMatuYld(HIS_DATA_TYPE data_type);
  void WriteIndexPosFile(const int64 unit_time, HIS_DATA_TYPE data_type);
  int16 WriteGoogleFile(const int64 unix_time, const char* his_data_type,
                        const char* his_data_suffix,
                        const std::string& content);

  chaos_data::SymbolStatic_SYMBOL_SUBTYPE GetSymbolSubType(const char sub_type);
  chaos_data::SymbolStatic_SYMBOL_SP GetSymbolSP(const char sp);
};

}

#endif
