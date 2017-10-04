//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_HIS_HEAD_H__
#define CHAOS_FC_HIS_HEAD_H__

#pragma once
//#pragma pack (push,1)

#include "basic/basictypes.h"
#include "mword.h"

#pragma pack(push)
#pragma pack (1)
struct GUID {
  uint32 data1_;
  uint16 data2_;
  uint16 data3_;
  uint8 data4_[8];
};
//#pragma pack(pop)
const struct GUID GUID_FC_HISDATA = { 0x2f872015, 0xb46e, 0x4d60, { 0x89, 0x60,
    0xb6, 0xb6, 0x6c, 0x18, 0x5a, 0xff } };

//通用结构
struct UINT24 {
  uint16 low_;
  unsigned char high_;
  uint32 Get() const {
    return low_ + (high_ << 16);
  }
  void Set(uint32 dw) {
    low_ = (uint16) dw;
    high_ = (unsigned char) (dw >> 16);
  }
  void operator=(uint32 dw) {
    Set(dw);
  }
  bool operator==(uint32 dw) const {
    return Get() == dw;
  }
  operator uint32() const {
    return Get();
  }
};

//市场定义

#define  MKT_SH   0x4853 //'HS'  // 上海证券交易所
#define  MKT_SZ   0x5A53 //'ZS'  // 深圳证券交易所
#define  MKT_HK   0x4B48 //'KH'  // 香港联交所
#define  MKT_TW   0x5754 //'WT'  // 台湾证券交易所
#define  MKT_NY   0x594E //'YN'  // 纽约证券交易所
#define  MKT_NSQ  0x534E //'SN'  // Nasdaq证券交易所

#define  MKT_FE   0x4546 //'EF'  // 外汇

#define  MKT_SC   0x4353 //'CS'  // 上海期交所
#define  MKT_ZC   0x435A //'CZ'  // 郑州期交所
#define  MKT_DC   0x4344 //'CD'  // 大连期交所
#define  MKT_CC   0x4343 //'CC'  // 芝加哥期货
#define  MKT_SF   0x4653 //'FS'  //上海金融期货交易所
#define  MKT_SG   0x4753 //'GS'  //上海黄金现货交易所
#define  MKT_BI   0x2442 //'$B'  // 板块指数
#define  MKT_UI   0x2424 //'$$'  // 自定义指数
#define  MKT_FI   0x4946 //'IF'  // 上交所固定收益平台
#define  MKT_IX   0x5849 //'XI'  // 全球主要市场指数
#define  MKT_ZI   0x495A //'IZ'  // 中证指数
#define  MKT_NW   0x574E //'WN'  // 新闻市场
#define  MKT_HS   0x5348 //'SH'  // H股市场，上交所提供的H股股价市场
#define  MKT_BO   0x4F42 //'OB'  // 渤海商品交易所
#define  MKT_DSM   0x2323 //'##'  //监控客户端
#define  MKT_SS   0x5353 //'SS'  //上海航运市场
#define  MKT_Z$   0x245A //'$Z'  //深圳全队列市场
#define  MKT_TE   0x4554 //'ET'  // 台湾柜买中心

//动态行情
struct STK_DYNA {
  uint16 stk_id_;			//股票ID
  int32 time_;			//成交时间
  uint32 open_;			//开盘
  uint32 high_;			//最高
  uint32 low_;			//最低
  uint32 new_;			//最新
  uint32 volume_;			//成交量
  MWORD amount_;			//成交额
  MWORD inner_vol_;		//内盘成交量,<0表示该笔成交为主动卖，>=0表示主动买,绝对值表示内盘成交量
  uint32 tick_count_;		//累计成交笔数
  uint32 buy_price_[5];  //委买价格
  uint32 buy_vol_[5];		//委买量
  uint32 sell_price_[5];  //委卖价格
  uint32 sell_vol_[5];		//委卖量
  uint32 open_interest_;  //持仓量(期货期指特有)
  uint32 settle_price_;  //结算价(期货期指现货特有)
};

//上海 level2
//扩展买卖盘
#define SHL2_MMP_SIZE 5

struct SH_L2_MMPEX {
  uint16 stk_id_;      //股票ID
  uint32 avg_buy_price_;    //加权平均委买价格
  MWORD all_buy_vol_;     //委买总量
  uint32 avg_sell_price_;    //加权平均委卖价格
  MWORD all_sell_vol_;     //委卖总量
  uint32 buy_price_[SHL2_MMP_SIZE];  //委买价6-10
  uint32 buy_vol_[SHL2_MMP_SIZE];  //委买量6-10
  uint32 sell_price_[SHL2_MMP_SIZE];  //委卖价6-10
  uint32 sell_vol_[SHL2_MMP_SIZE];  //委卖量6-10
};
//委托队列
//委托队列类型码定义，由每个DWORD的最高2位决定它的类型，每个股票的队列数据顺序为
//  股票ID | 买盘价格1 | 价格1总单数 | 买单1 | 买单2 ... | 买盘价格2 | 价格2总单数 | 买单1 | 买单2 ... | 卖盘价格1 | 价格1总单数 | 卖单1 | 卖单2 ...
//价格后面总是跟着一个总单数

//扩展买卖盘数据
struct HIS_L2_MMPEX {
  uint32 time_;  //时间
  SH_L2_MMPEX data_;  //数据
};

//逐笔成交明细
struct SH_L2_REPORT {
  uint16 stk_id_;
  int32 time_;    //成交时间
  uint32 price_;   //成交价格
  uint32 volume_;   //成交量
};

//委托单统计，用于深圳Level2
struct SZ_L2_ORDER_STAT {
  uint16 stk_id_;    //股票ID
  MWORD buy_order_vol_[4];  //买入单量，小、中、大、特大
  MWORD sell_order_vol_[4];  //卖出单量，小、中、大、特大
  UINT24 order_num_[2];  //委托单数量，买入/卖出
  MWORD vol_[2];   //买入/卖出撤单量,现在暂时将无法判明方向的总撤单量各送一半

  void Init(uint16 stk_id);  //初始化并清零数据
  void AddNewOrder(int dir, uint32 price_, uint32 vol,
                   unsigned char price_digit);  //新增委托单时调用，nPriceDigit引用 STK_STATIC::m_nPriceDigit,nDir>0表示买单，<0表示卖单
  void AddNewWithdarw(int dir, uint32 vol);  //新增成功撤单时调用，nDir=0表示不明方向，1表示买入撤单，-1表示卖出撤单
};

// 买卖队列数据
struct HIS_L2_ORDER_STAT {
  unsigned int time_;   // 时间
  SZ_L2_ORDER_STAT data_;   // 数据
};

// 动态净值估值
struct HIS_IOPV {
  unsigned int time_;   // 时间
  float value_;   // 数据
};

// 债券到期收益率
struct HIS_Matu_Yld {
  unsigned int time_;   // 时间
  float value_;   // 数据
};

struct HIS_FILE_HEAD {
  uint32 size_;   //文件头大小
  uint16 market_;   //市场
  uint32 market_date_;   //静态库日期 //YYYYMMDD
  uint16 stk_count_;   //证券数量
  uint32 reserved_;   //保留字段
};

//静态数据，码表
struct STK_STATIC {
  enum STK_TYPE {
    INDEX = 0,				  //指数
    STOCK = 1,				  //股票
    FUND = 2,				    //基金
    BOND = 3,				    //债券
    OTHER_STOCK = 4,		//其它股票
    OPTION = 5,				  //选择权
    EXCHANGE = 6,			  //外汇
    FUTURE = 7,				  //期货
    FTR_IDX = 8,			  //期指
    RGZ = 9,				    //认购证
    ETF = 10,				    //ETF
    LOF = 11,				    //LOF
    COV_BOND = 12,			//可转债
    TRUST = 13,				  //信托
    WARRANT = 14,			  //权证
    REPO = 15,				  //回购
    COMM = 16,				  //商品现货
  };

  uint16 stk_id_;			  //本市场内唯一标示,在本市场内的序号
  char symbol_[10];		//代码

  char name_[32];		//名称
  uint8 ctype_;			//STK_TYPE
  uint8 price_digit_;		//价格最小分辨率，非常重要，每一个DWORD类型的价格都要除以10^m_nPriceDigit才是真正的价格
  int16 vol_unit_;			//成交量单位，每一成交量单位表示多少股

  MWORD float_issued_;		//流通股本
  MWORD total_issued_;		//总股本

  uint32 last_close_;		//昨收
  uint32 adv_stop_;		//涨停
  uint32 dec_stop_;		//跌停

  int GetPriceMul() const;		//价格乘数，由price_digit_决定
};

//扩展静态数据,作为静态数据的补充，不是每一个股票都必须的，可以选择性生成部分股票的数据，在发送STK_STATIC后发送,使用DCT_GENERAL发送
struct STK_STATICEx {
  enum STK_SUBTYPE {
    NILTYPE = 0,
    ASHARE = 'A',			//A股,仅对STOCK,WARRANT有效
    BSHARE = 'B',			//B股,仅对STOCK,WARRANT有效
    GOV_BOND = 'G',			//国债,仅对BOND有效
    ENT_BOND = 'O',			//企业债,仅对BOND有效
    FIN_BOND = 'F',			//金融债,仅对BOND有效
  };
  enum STK_SP			//股票属性
  {
    NULLSP = 0,
    NSP = 'N',			//正常
    SSP = 'S',			//ST股
    PSP = 'P',			//PT股
    TSP = 'T',			//代办转让证券
    LSP = 'L',			//上市开放型基金（LOF）
    OSP = 'O',			//仅揭示净值的开放式基金
    FSP = 'F',			//非交易型开放式基金
    ESP = 'E',			//ETF
    ZSP = 'Z',			//处于退市整理期的证券
  };
  char ctype_;			//STK_TYPE,由该标志决定联合中使用哪一个结构
  char csub_type_;			//STK_SUBTYPE
  union {
    struct		//股票	(STOCK,OTHER_STOCK)
    {
      float face_value_;		//	面值
      float profit_;			//	每股收益
      uint16 industry_;		//	行业'A' -- 'M',参见 industryClasify
      char trade_status_;  //	交易状态，'N'=Normal, 'H'=Halt, 'X'=Not trade on this Market
      float cash_dividend_;  //	每股红利
      char security_properties_;	//特殊分类标记，取值集合STK_SP中的值
      uint32 last_tradedate_;  //最后交易日，目前为空，做预留用

    } equity_spec_;
    struct		//基金,ETF,LOF	(FUND,ETF,LOF)
    {
      float face_value_;		//	面值
      float total_issued_;		//	总股本
      float iopv_;			//	IOPV净值,仅对ETF,LOF有效，***
    } fund_spec_;
    struct		//权证,选择权	(OPTION,WARRANT)
    {
      char style_;			//  'A' or 'E'	American or European Style
      char cp_;				//	'C' or 'P' Call or Put
      float cnvt_ratio_;		//  兑换/行权比例
      float strike_price_;		//	行权价格
      uint32 maturity_date_;  //	到期日,YYYYMMDD
      char under_line_[12];  //	对应股票,包含市场代码，如SH600000
      float balance_;			//  流通余额
    } warrant_spec_;
    struct		//债券	(BOND)
    {
      uint32 maturity_date_;  //	到期日,YYYYMMDD
      uint32 intaccru_date_;  //	起息日，YYYYMMDD
      float isssue_price_;		//	发行价
      float coupon_rate_;		//	利率
      float face_value_;		//	面值
      float accrued_int_;		//	应计利息,***
    } bond_spec_;
    struct		//可转债	(COV_BOND)
    {
      char style_;			//  'A' or 'E'	American or European Style
      char cp_;				//	'C' or 'P' Call or Put
      float cnvt_ratio_;		//  兑换/行权比例
      float strike_price_;		//	行权价格
      uint32 maturity_date_;  //	到期日,YYYYMMDD
      char under_line_[12];  //	对应股票,包含市场代码，如SH600000
      float accrued_int_;		//	应计利息
    } cnvt_spec_;
    struct		//期货,商品	(FUTURE,FTR_IDX,COMM)
    {
      uint32 last_day_OI_;			//昨日持仓量
      float last_settle_price_;		//昨日结算价
    } future_spec_;
    struct		//信托	(TRUST)
    {
      float asset_;			//净资产
      uint32 asset_date_;		//YYYYMMDD
    } trust_spec_;
  };
};

//历史数据类型
enum HIS_DATA_TYPE {
  _STATIC_DATA = 0,			//	静态数据
  _DYNA_DATA,						//	动态行情数据
  _L2_MMPEX,						//	level2扩展买卖盘
  _L2_REPORT,						//	level2逐笔成交
  _L2_ORDER_STAT,					//	委托队列
  _IOPV,							//	动态净值估值
  _MATU_YLD,						//	债券到期收益率
  HIS_DATA_TYPE_COUNT,			//	历史数据类型总数
  HIS_DATA_TYPE_UNKNOWN			//	未知历史数据类型
};

struct HIS_DATA_HEAD {
  uint16 head_size_;		//	包头大小
  uint16 item_size_;		//	数据单元大小
  uint32 item_count_;		//	数据单元个数
  uint32 package_flag_;  //	包特征
  uint32 package_size_;  //	包大小
};

//	历史数据压缩格式
enum HISHD_COMPRESS_TYPE {
  CT_NONE = 0,					//	无压缩
  CT_ZLIB,						//	ZLIB压缩

  CT_MASKCOUNT,					//	压缩方式总数
  CT_MASK = 0xFF					//	压缩标志掩码
};
#pragma pack(pop)
extern const char* g_data_compress_type[CT_MASKCOUNT];

extern const char* g_his_data_suffix[HIS_DATA_TYPE_COUNT];

extern const char* g_his_data_type[HIS_DATA_TYPE_COUNT];

extern const char* g_his_data_type_en[HIS_DATA_TYPE_COUNT];

extern const char* s_stk_type[];
extern const char* s_stk_type_en[];

#endif
