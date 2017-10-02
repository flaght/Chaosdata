//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#include "his_head.h"
#include "flw_his_stk.h"
#include "stk_datacps.h"
#include "logic/logic_comm.h"
#include "basic/basic_util.h"
#include "file/file_path.h"
#include "file/file_util.h"
#include "protocol/data_packet.h"
namespace fc_data {

const chaos_data::SymbolStatic_SYMBOL_TYPE g_gpb_data_type[chaos_data::SymbolStatic_SYMBOL_TYPE_SYMBOL_TYPE_ARRAYSIZE] =
    { chaos_data::SymbolStatic_SYMBOL_TYPE_INDEX,
        chaos_data::SymbolStatic_SYMBOL_TYPE_STOCK,
        chaos_data::SymbolStatic_SYMBOL_TYPE_FUND,
        chaos_data::SymbolStatic_SYMBOL_TYPE_BOND,
        chaos_data::SymbolStatic_SYMBOL_TYPE_OTHER_STOCK,
        chaos_data::SymbolStatic_SYMBOL_TYPE_OPTION,
        chaos_data::SymbolStatic_SYMBOL_TYPE_EXCHANGE,
        chaos_data::SymbolStatic_SYMBOL_TYPE_FUTURE,
        chaos_data::SymbolStatic_SYMBOL_TYPE_FTR_IDX,
        chaos_data::SymbolStatic_SYMBOL_TYPE_RGZ,
        chaos_data::SymbolStatic_SYMBOL_TYPE_ETF,
        chaos_data::SymbolStatic_SYMBOL_TYPE_LOF,
        chaos_data::SymbolStatic_SYMBOL_TYPE_COV_BOND,
        chaos_data::SymbolStatic_SYMBOL_TYPE_TRUST,
        chaos_data::SymbolStatic_SYMBOL_TYPE_WARRANT,
        chaos_data::SymbolStatic_SYMBOL_TYPE_REPO,
        chaos_data::SymbolStatic_SYMBOL_TYPE_COMM };

FlwHisStk::FlwHisStk(std::string& dir)
    : static_size_(NULL),
      static_(NULL),
      static_ex_(NULL),
      staticex_size_(NULL),
      his_data_count_(NULL),
      data_ptr_(NULL),
      data_head_(NULL),
      market_date_(0),
      out_dir_(dir){
  
   market_mtk_ = {0};
}

FlwHisStk::~FlwHisStk() {
  RestStk();
  while(his_data_list_.size() > 0) {
      fc_data::FlwHisData* flw_data = his_data_list_.front();
      his_data_list_.pop_front();
      if (flw_data) {delete flw_data; flw_data = NULL;}
  }
  //LOG_MSG2("his_data_list size %d",his_data_list_.size());
}

//清除数据
void FlwHisStk::RestStk() {
  static_size_ = NULL;
  static_ = NULL;
  static_ex_ = NULL;
  staticex_size_ = NULL;
  his_data_count_ = NULL;
}

bool FlwHisStk::LoadStk(const unsigned char*& his_buffer,
                        const uint32 market_date, const uint16 market_mtk) {
  RestStk();

  try {
    static_size_ = (const int32*) his_buffer;
    his_buffer = (const unsigned char*) (static_size_ + 1);

    if (*static_size_ < sizeof(struct STK_STATIC))
      return false;

    static_ = (const struct STK_STATIC*) his_buffer;
    his_buffer = his_buffer + (*static_size_);

    staticex_size_ = (const int32*) his_buffer;
    his_buffer = (const unsigned char*) (staticex_size_ + 1);

    if (*staticex_size_ < sizeof(struct STK_STATICEx))
      return false;

    static_ex_ = (const struct STK_STATICEx*) his_buffer;
    his_buffer = his_buffer + (*staticex_size_);

    his_data_count_ = (const int32*) his_buffer;
    his_buffer = (const unsigned char*) (his_data_count_ + 1);

    if (*his_data_count_ < 0)
      return false;

    market_date_ = market_date;
    memcpy(market_mtk_, &market_mtk, sizeof(uint16));

    if (*his_data_count_) {
      for (int i = 0; i < (*his_data_count_); i++) {
        fc_data::FlwHisData* flw_data = new fc_data::FlwHisData();
        if (!flw_data->LoadData(his_buffer)) {
          RestStk();
          return false;
        }
        his_data_list_.push_back(flw_data);
      }
    }
  } catch (...) {
    RestStk();
    return false;
  }
  return true;
}

//获取证券代码
bool FlwHisStk::GetStkStatic(struct STK_STATIC& stk_code) {
  if (NULL == static_)
    return false;
  memcpy(&stk_code, static_, sizeof(struct STK_STATIC));
  return true;
}

//读取证券数据类型列表
bool FlwHisStk::GetDataTypeList(std::vector<int>& list) {
  if (NULL == static_)
    return false;
  HISDATA_LIST::iterator it = his_data_list_.begin();
  while (it != his_data_list_.end()) {
    list.push_back((*it)->his_data_type_);
    it++;
  }
  return true;
}

bool FlwHisStk::ProcessHisTypeList() {
  if (NULL == static_)
    return false;
  HISDATA_LIST::iterator it = his_data_list_.begin();
  while (it != his_data_list_.end()) {
    ProcessTypeData((*it));
    it++;
  }
  return true;
}

void FlwHisStk::WriteStatic(HIS_DATA_TYPE data_type, const int32 year, const int32 month,
                            const int32 day) {
  chaos_data::SymbolStatic symbol_static;
  std::string content;
  symbol_static.set_symbol(static_->symbol_);
  symbol_static.set_name(static_->name_);
  symbol_static.set_ctype(g_gpb_data_type[static_->ctype_]);
  symbol_static.set_csub_type(GetSymbolSubType(static_ex_->csub_type_));
  symbol_static.set_price_digit(static_->price_digit_);
  symbol_static.set_vol_unit(static_->vol_unit_);
  symbol_static.set_float_issued(static_->float_issued_.GetValue());
  symbol_static.set_total_issued(static_->total_issued_.GetValue());
  symbol_static.set_last_close(static_->last_close_);
  symbol_static.set_adv_stop(static_->adv_stop_);
  symbol_static.set_dec_stop(static_->dec_stop_);

  chaos_data::SymbolStatic::SpecMessage* spc = symbol_static.mutable_spec();

  switch (static_->ctype_) {
    case STK_STATIC::INDEX: {  //指数
      break;
    }
    case STK_STATIC::STOCK:
    case STK_STATIC::OTHER_STOCK: {  //股票//其他股票
      chaos_data::SymbolStatic::EquityMessage* equity = spc->mutable_equity();
      equity->set_face_value(static_ex_->equity_spec_.face_value_);
      equity->set_profit(static_ex_->equity_spec_.profit_);
      equity->set_industry(static_ex_->equity_spec_.industry_);
      equity->set_trade_status(static_ex_->equity_spec_.trade_status_);
      equity->set_cash_dividend(static_ex_->equity_spec_.cash_dividend_);
      equity->set_security_properties(
          GetSymbolSP(static_ex_->equity_spec_.security_properties_));
      equity->set_last_tradedate(static_ex_->equity_spec_.last_tradedate_);
      break;
    }
    case STK_STATIC::FUND:  //基金
    case STK_STATIC::ETF:  //ETF
    case STK_STATIC::LOF: {  //LOF
      chaos_data::SymbolStatic::FundMessage* fund = spc->mutable_fund();
      fund->set_face_value(static_ex_->fund_spec_.face_value_);
      fund->set_total_issued(static_ex_->fund_spec_.total_issued_);
      fund->set_iopv(static_ex_->fund_spec_.iopv_);
      break;
    }
    case STK_STATIC::BOND: {  //债券
      chaos_data::SymbolStatic::BoundMessage* bound = spc->mutable_bound();
      bound->set_maturity_date(static_ex_->bond_spec_.maturity_date_);
      bound->set_intaccru_date(static_ex_->bond_spec_.intaccru_date_);
      bound->set_isssue_price(static_ex_->bond_spec_.isssue_price_);
      bound->set_coupon_rate(static_ex_->bond_spec_.coupon_rate_);
      bound->set_face_value(static_ex_->bond_spec_.face_value_);
      bound->set_accrued_int(static_ex_->bond_spec_.accrued_int_);
      break;
    }
    case STK_STATIC::OPTION:  //选择权
    case STK_STATIC::WARRANT: {  //权证
      chaos_data::SymbolStatic::WarranMessage* warran = spc->mutable_warran();
      warran->set_style(static_ex_->warrant_spec_.style_);
      warran->set_cp(static_ex_->warrant_spec_.cp_);
      warran->set_cnvt_ratio(static_ex_->warrant_spec_.cnvt_ratio_);
      warran->set_strike_price(static_ex_->warrant_spec_.strike_price_);
      warran->set_maturity_date(static_ex_->warrant_spec_.maturity_date_);
      warran->set_under_line(static_ex_->warrant_spec_.under_line_);
      warran->set_balance(static_ex_->warrant_spec_.balance_);
      break;
    }
    case STK_STATIC::EXCHANGE: {  //外汇
      break;
    }
    case STK_STATIC::FUTURE:  //期货
    case STK_STATIC::COMM:  //商品现货
    case STK_STATIC::FTR_IDX: {  //期指
      chaos_data::SymbolStatic::FutureMessage* future = spc->mutable_future();
      future->set_last_day_oi(static_ex_->future_spec_.last_day_OI_);
      future->set_last_settle_price(
          static_ex_->future_spec_.last_settle_price_);
      break;
    }
    case STK_STATIC::RGZ: {  //认购证
      break;
    }
    case STK_STATIC::COV_BOND: {  //可转债
      chaos_data::SymbolStatic::CNVTMessage* cnvt = spc->mutable_cnvt();
      cnvt->set_style(static_ex_->cnvt_spec_.style_);
      cnvt->set_cp(static_ex_->cnvt_spec_.cp_);
      cnvt->set_cnvt_ratio(static_ex_->cnvt_spec_.cnvt_ratio_);
      cnvt->set_strike_price(static_ex_->cnvt_spec_.strike_price_);
      cnvt->set_maturity_date(static_ex_->cnvt_spec_.maturity_date_);
      cnvt->set_under_line(static_ex_->cnvt_spec_.under_line_);
      cnvt->set_accrued_int(static_ex_->cnvt_spec_.accrued_int_);
      break;
    }
    case STK_STATIC::TRUST: {  //信托
      chaos_data::SymbolStatic::TruestMessage* truset = spc->mutable_truest();
      truset->set_asset(static_ex_->trust_spec_.asset_);
      truset->set_asset_date(static_ex_->trust_spec_.asset_date_);
      break;
    }
    case STK_STATIC::REPO: {  //回购
      break;
    }
    default:
      break;
  }

  std::string in_data;
  bool r = symbol_static.SerializeToString(&in_data);
  if (!r) {
    LOG_ERROR2("symbol %s STK_STATIC GoogleProtoBuffer error ", static_->symbol_);
    return;
  }

  std::string dir = out_dir_ + "/" + std::string(s_stk_type_en[static_->ctype_])
        + "/" + std::string(market_mtk_) + "/" + std::string(static_->symbol_)
        + "/" + std::string(g_his_data_type_en[data_type]) 
        + "/" + base::BasicUtil::StringUtil::Int64ToString(year)
        + "/" + base::BasicUtil::StringUtil::Int64ToString(month);
  
  file::FilePath current_dir_path(dir);
  if (!file::DirectoryExists(current_dir_path))
    file::CreateDirectory(current_dir_path);
  
  std::string file_name = std::string(market_mtk_) + "_"
  + std::string(static_->symbol_) + "_" 
      + base::BasicUtil::StringUtil::Int64ToString(year)
           + base::BasicUtil::StringUtil::Int64ToString(month)
      + base::BasicUtil::StringUtil::Int64ToString(day);
  std::string temp_path = current_dir_path.value() + "/" + file_name + std::string(g_his_data_suffix[0]) + ".chspb";

  file::FilePath temp_file_path(temp_path);
  //檢測是否存在
  file::DirectoryExists(temp_file_path);
  file::WriteFile(temp_file_path, content.c_str(), content.length());
}

void FlwHisStk::WriteDynaData(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct STK_DYNA* dyna_data = (const STK_DYNA*) (data_ptr_);
    chaos_data::SymbolDynamMarket dynam_markert;
    dynam_markert.set_current_time(dyna_data->time_);
    dynam_markert.set_open_price(dyna_data->open_);
    dynam_markert.set_high_price(dyna_data->high_);
    dynam_markert.set_low_price(dyna_data->low_);
    dynam_markert.set_new_price(dyna_data->new_);
    dynam_markert.set_volume(dyna_data->volume_);
    dynam_markert.set_amount(dyna_data->amount_.GetValue());
    dynam_markert.set_inner_vol(dyna_data->inner_vol_.GetValue());
    dynam_markert.set_tick_count(dyna_data->tick_count_);

    //----->
    dynam_markert.add_buy_price(dyna_data->buy_price_[0]);
    dynam_markert.add_buy_vol(dyna_data->buy_vol_[0]);
    dynam_markert.add_sell_price(dyna_data->sell_price_[0]);
    dynam_markert.add_sell_vol(dyna_data->sell_vol_[0]);

    //----->
    dynam_markert.add_buy_price(dyna_data->buy_price_[1]);
    dynam_markert.add_buy_vol(dyna_data->buy_vol_[1]);
    dynam_markert.add_sell_price(dyna_data->sell_price_[1]);
    dynam_markert.add_sell_vol(dyna_data->sell_vol_[1]);

    //----->
    dynam_markert.add_buy_price(dyna_data->buy_price_[2]);
    dynam_markert.add_buy_vol(dyna_data->buy_vol_[2]);
    dynam_markert.add_sell_price(dyna_data->sell_price_[2]);
    dynam_markert.add_sell_vol(dyna_data->sell_vol_[2]);

    //----->
    dynam_markert.add_buy_price(dyna_data->buy_price_[3]);
    dynam_markert.add_buy_vol(dyna_data->buy_vol_[3]);
    dynam_markert.add_sell_price(dyna_data->sell_price_[3]);
    dynam_markert.add_sell_vol(dyna_data->sell_vol_[3]);

    //----->
    dynam_markert.add_buy_price(dyna_data->buy_price_[4]);
    dynam_markert.add_buy_vol(dyna_data->buy_vol_[4]);
    dynam_markert.add_sell_price(dyna_data->sell_price_[4]);
    dynam_markert.add_sell_vol(dyna_data->sell_vol_[4]);
    std::string in_data = "";
    bool r = dynam_markert.SerializeToString(&in_data);
    if (r && !in_data.empty()) {    //写入文件
      WriteGoogleFile(dyna_data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s DYNA GoogleProtoBuffer Error length:%d", 
                 static_->symbol_,in_data.length());
    }
    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteL2MMPEX(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct HIS_L2_MMPEX* data = (const struct HIS_L2_MMPEX*) (data_ptr_);
    chaos_data::SymbolL2MMPEX l2_mmpex;
    l2_mmpex.set_time(data->time_);
    l2_mmpex.set_avg_buy_price(data->data_.avg_buy_price_);
    l2_mmpex.set_all_buy_vol(data->data_.all_buy_vol_.GetValue());
    l2_mmpex.set_avg_sell_price(data->data_.avg_sell_price_);
    l2_mmpex.set_all_sell_vol(data->data_.all_sell_vol_.GetValue());
//----->
    l2_mmpex.add_buy_price(data->data_.buy_price_[0]);
    l2_mmpex.add_buy_vol(data->data_.buy_vol_[0]);
    l2_mmpex.add_sell_price(data->data_.sell_price_[0]);
    l2_mmpex.add_sell_vol(data->data_.sell_vol_[0]);

//----->
    l2_mmpex.add_buy_price(data->data_.buy_price_[1]);
    l2_mmpex.add_buy_vol(data->data_.buy_vol_[1]);
    l2_mmpex.add_sell_price(data->data_.sell_price_[1]);
    l2_mmpex.add_sell_vol(data->data_.sell_vol_[1]);

//----->
    l2_mmpex.add_buy_price(data->data_.buy_price_[2]);
    l2_mmpex.add_buy_vol(data->data_.buy_vol_[2]);
    l2_mmpex.add_sell_price(data->data_.sell_price_[2]);
    l2_mmpex.add_sell_vol(data->data_.sell_vol_[2]);

//----->
    l2_mmpex.add_buy_price(data->data_.buy_price_[3]);
    l2_mmpex.add_buy_vol(data->data_.buy_vol_[3]);
    l2_mmpex.add_sell_price(data->data_.sell_price_[3]);
    l2_mmpex.add_sell_vol(data->data_.sell_vol_[3]);
//----->
    l2_mmpex.add_buy_price(data->data_.buy_price_[4]);
    l2_mmpex.add_buy_vol(data->data_.buy_vol_[4]);
    l2_mmpex.add_sell_price(data->data_.sell_price_[4]);
    l2_mmpex.add_sell_vol(data->data_.sell_vol_[4]);
    std::string in_data;
    bool r = l2_mmpex.SerializeToString(&in_data);

    if (r && !in_data.empty()) {    //写入文件
      WriteGoogleFile(data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s HIS_L2_MMPEX GoogleProtoBuffer Error length %d", 
                 static_->symbol_,in_data.length());
    }

    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteL2Report(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct SH_L2_REPORT *data = (const struct SH_L2_REPORT*) (data_ptr_);
    chaos_data::SymbolL2Report l2_report;
    l2_report.set_time(data->time_);
    l2_report.set_price(data->price_);
    l2_report.set_volume(data->volume_);
    std::string in_data;
    bool r = l2_report.SerializeToString(&in_data);
    
    if (r && !in_data.empty()) {    //写入文件
      WriteGoogleFile(data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s HIS_L2_MMPEX GoogleProtoBuffer Error length:%d", 
                 static_->symbol_, in_data.length());
    }
    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteOrderStat(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct HIS_L2_ORDER_STAT* data =
        (const struct HIS_L2_ORDER_STAT*) (data_ptr_);
    chaos_data::SymbolL2OrderState l2_order;
    l2_order.set_time(data->time_);
    //chaos_data::SymbolStatic::SpecMessage* spc = symbol_static.mutable_spec();
    chaos_data::SymbolL2OrderState::SymbolOrderStat* order_state = l2_order
        .mutable_order_state();

    order_state->add_buy_order_vol(data->data_.buy_order_vol_[0].GetValue());
    order_state->add_buy_order_vol(data->data_.buy_order_vol_[1].GetValue());
    order_state->add_buy_order_vol(data->data_.buy_order_vol_[2].GetValue());
    order_state->add_buy_order_vol(data->data_.buy_order_vol_[3].GetValue());

    order_state->add_sell_order_vol(data->data_.sell_order_vol_[0].GetValue());
    order_state->add_sell_order_vol(data->data_.sell_order_vol_[1].GetValue());
    order_state->add_sell_order_vol(data->data_.sell_order_vol_[2].GetValue());
    order_state->add_sell_order_vol(data->data_.sell_order_vol_[3].GetValue());

    order_state->add_order_num(data->data_.order_num_[0].Get());
    order_state->add_order_num(data->data_.order_num_[1].Get());

    order_state->add_vol(data->data_.vol_[0].GetValue());
    order_state->add_vol(data->data_.vol_[1].GetValue());
    std::string in_data;
    bool r = l2_order.SerializeToString(&in_data);
    
    if (r && !in_data.empty()) {    //写入文件
      WriteGoogleFile(data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s HIS_L2_MMPEX GoogleProtoBuffer Error length:%d", 
                 static_->symbol_, in_data.length());
    }

    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteIOPV(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct HIS_IOPV* data = (const struct HIS_IOPV*) (data_ptr_);
    chaos_data::SymbolHisIOPV iopv;
    iopv.set_time(data->time_);
    iopv.set_value(data->value_);
    std::string in_data;
    bool r = iopv.SerializeToString(&in_data);
    
    if (r && !in_data.length()) {    //写入文件
      WriteGoogleFile(data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s HIS_L2_MMPEX GoogleProtoBuffer Error length:%d", 
                 static_->symbol_,in_data.length());
    }
    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteMatuYld(HIS_DATA_TYPE data_type) {
  int32 count = data_head_->item_count_;
  int32 index = 0;
  while (index < count) {
    const struct HIS_Matu_Yld* data = (const struct HIS_Matu_Yld*) (data_ptr_);
    chaos_data::SymbolHisMatuYld matu_yld;
    matu_yld.set_time(data->time_);
    matu_yld.set_value(data->value_);
    std::string in_data;
    bool r = matu_yld.SerializeToString(&in_data);
    
    if (r && !in_data.empty()) {    //写入文件
      WriteGoogleFile(data->time_, data_type, in_data);
    } else {
      LOG_ERROR2("symbol:%s HIS_L2_MMPEX GoogleProtoBuffer Error length:%d", 
                 static_->symbol_, in_data.length());
    }
    index++;
    data_ptr_ = data_ptr_ + data_head_->item_size_;
  }
}

void FlwHisStk::WriteGoogleFile(const int64 unix_time, HIS_DATA_TYPE data_type,
                                const std::string& content) {
  time_t u_time = unix_time;
  struct tm *local_time = localtime(&u_time);
  int16 packet_length = content.length() + sizeof(int16);
  packet::DataOutPacket out(true, packet_length);
  out.Write16(packet_length);
  out.WriteData(content.c_str(), content.length());
  std::string dir = out_dir_+ "/" + std::string(s_stk_type_en[static_->ctype_])
      + "/" + std::string(market_mtk_)
      + "/" + std::string(static_->symbol_)
      + "/" + std::string(g_his_data_type_en[data_type])
      + "/" + base::BasicUtil::StringUtil::Int64ToString(local_time->tm_year + 1900)
      + "/"
      + base::BasicUtil::StringUtil::Int64ToString(local_time->tm_mon + 1);
//+ "/" + base::BasicUtil::StringUtil::Int64ToString(day);
  file::FilePath current_dir_path(dir);
  if (!file::DirectoryExists(current_dir_path))
    file::CreateDirectory(current_dir_path);
  std::string file_name = std::string(market_mtk_) + "_"
      + std::string(static_->symbol_) + "_"
      + base::BasicUtil::StringUtil::Int64ToString(local_time->tm_year + 1900)
      + base::BasicUtil::StringUtil::Int64ToString(local_time->tm_mon + 1)
      + base::BasicUtil::StringUtil::Int64ToString(local_time->tm_mday);
  std::string temp_path = current_dir_path.value() + "/" + file_name  +  std::string(g_his_data_suffix[data_type])
                + ".chspb";

  file::FilePath temp_file_path(temp_path);
  //檢測是否存在
  file::DirectoryExists(temp_file_path);
  file::WriteFile(temp_file_path, content.c_str(), content.length());
}

void FlwHisStk::ProcessTypeData(fc_data::FlwHisData* flw_data) {
  int buffer_length = flw_data->ReadData(NULL, 0);
  if (buffer_length > 0) {
    std::string src_buf;
    src_buf.resize(buffer_length);
    int32 nread = flw_data->ReadData((char*) src_buf.c_str(), buffer_length);
    if (nread) {
      const struct HIS_DATA_HEAD* data_head =
          (const struct HIS_DATA_HEAD*) (src_buf.c_str());
      if (data_head->package_flag_ & CT_MASK) {
        int32 nbuff_length = fc_data::UnCompressData(
            (const unsigned char*) src_buf.c_str(), NULL, 0);
        if (nbuff_length) {
          data_buffer_.resize(0);
          data_buffer_.resize(nbuff_length);
          char* buf = (char*) data_buffer_.c_str();
          if (fc_data::UnCompressData((const unsigned char*) src_buf.c_str(),
                                      buf, nbuff_length) > 0) {
            data_head_ = (struct HIS_DATA_HEAD*) buf;
            data_ptr_ = buf + data_head_->head_size_;
          }
        }
      } else {
        data_buffer_.resize(0);
        data_buffer_.resize(buffer_length);
        char* buf = (char*) data_buffer_.c_str();
        memcpy(buf, src_buf.c_str(), buffer_length);
        data_head_ = (struct HIS_DATA_HEAD*) buf;
        data_ptr_ = buf + data_head_->head_size_;
      }

    }

    src_buf.resize(0);
  }
  
    //存储静态数据
    WriteStatic(flw_data->his_data_type_, market_date_/10000, (market_date_/100)%100, market_date_%100);
  switch (flw_data->his_data_type_) {
    case _DYNA_DATA:
      WriteDynaData(flw_data->his_data_type_);
      break;
    case _L2_MMPEX:
      WriteL2MMPEX(flw_data->his_data_type_);
      break;
    case _L2_REPORT:
      WriteL2Report(flw_data->his_data_type_);
      break;
    case _L2_ORDER_STAT:
      WriteOrderStat(flw_data->his_data_type_);
      break;
    case _IOPV:
      WriteIOPV(flw_data->his_data_type_);
      break;
    case _MATU_YLD:
      WriteMatuYld(flw_data->his_data_type_);
      break;
    default:
      break;
  }
}

chaos_data::SymbolStatic_SYMBOL_SUBTYPE FlwHisStk::GetSymbolSubType(
    const char sub_type) {
  int8 c_sub_type = sub_type;
  switch (c_sub_type) {
    case chaos_data::SymbolStatic_SYMBOL_SUBTYPE_ASHARE:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_ASHARE;
    case chaos_data::SymbolStatic_SYMBOL_SUBTYPE_BSHARE:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_BSHARE;
    case chaos_data::SymbolStatic_SYMBOL_SUBTYPE_GOV_BOND:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_GOV_BOND;
    case chaos_data::SymbolStatic_SYMBOL_SUBTYPE_ENT_BOND:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_ENT_BOND;
    case chaos_data::SymbolStatic_SYMBOL_SUBTYPE_FIN_BOND:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_FIN_BOND;
    default:
      return chaos_data::SymbolStatic_SYMBOL_SUBTYPE_NILTYPE;
  }
}

chaos_data::SymbolStatic_SYMBOL_SP FlwHisStk::GetSymbolSP(const char sp) {
  int8 c_sp = sp;
  switch (c_sp) {
    case chaos_data::SymbolStatic_SYMBOL_SP_NSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_NSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_SSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_SSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_PSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_PSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_TSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_TSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_LSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_LSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_OSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_OSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_FSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_FSP;
    case chaos_data::SymbolStatic_SYMBOL_SP_ESP:
      return chaos_data::SymbolStatic_SYMBOL_SP_ESP;
    case chaos_data::SymbolStatic_SYMBOL_SP_ZSP:
      return chaos_data::SymbolStatic_SYMBOL_SP_ZSP;
    default:
      return chaos_data::SymbolStatic_SYMBOL_SP_NULLSP;
  }
}

}
