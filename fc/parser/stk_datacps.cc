#include "stk_datacps.h"
#include "zlib.h"
#include <algorithm>

const char* g_his_data_type[HIS_DATA_TYPE_COUNT] = { "静态数据", "动态行情",
    "level2扩展买卖盘", "level2逐笔成交", "委托队列", "动态净值估值", "债券到期收益率" };

const char* g_his_data_type_en[HIS_DATA_TYPE_COUNT] = { "STATIC", "DYNA",
    "L2_MMPEX", "L2_REPORT", "L2_ORDER_STAT", "HIS_IOPV", "MatuYld" };

const char* g_his_data_suffix[HIS_DATA_TYPE_COUNT] = { ".stc", ".dya", ".l2mx",
    ".l2rt", ".l2ost", ".iopv", ".mad" };

const char* s_stk_type[] = { "指数", "股票", "基金", "债券", "其它股票", "选择权", "外汇", "期货",
    "期指", "认购证", "ETF", "LOF", "可转债", "信托", "权证", "回购", "商品现货" };

const char* s_stk_type_en[] = { "index", "stock", "fund", "bond", "other_stock",
    "option", "exchange", "future", "ftr_idx", "RGZ", "ETF", "LOF", "cov_bond",
    "trust", "warrant", "repo", "comm" };

const char* g_data_compress_type[CT_MASKCOUNT] = { "无压缩", "ZLIB压缩" };

int STK_STATIC::GetPriceMul() const {
  int mul = std::min(6, (int) price_digit_);
  int n = 1;
  for (int i = 0; i < mul; i++)
    n *= 10;
  return n;
}

void SZ_L2_ORDER_STAT::Init(uint16 stk_id) {
  memset(this, 0, sizeof(SZ_L2_ORDER_STAT));
  stk_id_ = stk_id;
}

//新增委托单时调用，price_digit引用 STK_STATIC::price_digit
void SZ_L2_ORDER_STAT::AddNewOrder(int dir, uint32 price, uint32 vol,
                                   unsigned char price_digit) {
  static uint32 vol_limit[] = { 0, 20000, 100000, 500000 };
  static uint32 amount_limit[] = { 0, 40000, 100000, 1000000 };

  int i;
  uint32 amount = price * vol;
  while (price_digit--)
    amount /= 10;

  for (i = 3; i >= 0; i--) {
    if (vol >= vol_limit[i] || amount >= amount_limit[i]) {
      if (dir >= 0) {
        buy_order_vol_[i] += vol;
        order_num_[0] = order_num_[0] + 1;
      } else {
        sell_order_vol_[i] += vol;
        order_num_[1] = order_num_[1] + 1;
      }
      break;
    }
  }
}

//新增成功撤单时调用，dir=0表示不明方向，1表示买入撤单，-1表示卖出撤单,dwPrice不知道就填写0
void SZ_L2_ORDER_STAT::AddNewWithdarw(int dir, uint32 vol) {
  if (dir)
    vol_[dir > 0 ? 0 : 1] += vol;
  else {
    vol_[0] += vol / 2;
    vol_[1] += vol / 2;
  }
}

namespace fc_data {

bool IsEqualGUID(const struct GUID rguid, const struct GUID lguid) {
  return (((uint32*) &rguid)[0] == ((uint32*) &lguid)[0]
      && ((uint16*) &rguid)[1] == ((uint16*) &lguid)[1]
      && ((uint16*) &rguid)[2] == ((uint16*) &lguid)[2]
      && ((uint8*) &rguid)[3] == ((uint8*) &lguid)[3]);
}

//	解压数据
int UnCompressData(const unsigned char* src_buf		//	源数据
    , void* dst_buf		//	目标缓冲区
    , unsigned int buf_size		//	目标缓冲区大小
    ) {
  if (NULL == src_buf)
    return -1;

  const struct HIS_DATA_HEAD* data_head = (const struct HIS_DATA_HEAD*) src_buf;
  const unsigned char* src_data = src_buf + data_head->head_size_;

  uLongf dst_data_size = data_head->item_size_ * data_head->item_count_;
  uint32 dst_buf_size = data_head->head_size_ + dst_data_size;

  if (buf_size < dst_buf_size)
    return dst_buf_size;

  if (dst_buf == NULL)
    return -1;

  struct HIS_DATA_HEAD* dst_head = (struct HIS_DATA_HEAD*) dst_buf;
  unsigned char* dst_data = (unsigned char*) dst_buf + data_head->head_size_;

  memcpy(dst_head, data_head, data_head->head_size_);
  HISHD_COMPRESS_TYPE ctype = (HISHD_COMPRESS_TYPE) (data_head->package_flag_
      & CT_MASK);
  switch (ctype) {
    case CT_ZLIB: {
      if (Z_OK
          == uncompress(dst_data, &dst_data_size, src_data,
                        data_head->package_size_)) {
        dst_head->package_flag_ &= ~CT_MASK;
        dst_head->package_size_ = dst_data_size;
      } else
        return -1;
    }
      break;
    default:
      return -1;
  }

  return dst_data_size;
}

int CompressData(const unsigned char* src_buf  //源数据
    , const unsigned int src_buf_size  //源数据长度
    , HISHD_COMPRESS_TYPE ct_type  //压缩类型
    , void* dst_buf  //目标缓冲区
    , unsigned int buf_size  //目标缓冲区大小
    ) {
  switch (ct_type) {
    case CT_ZLIB: {
      uLongf bound = compressBound(src_buf_size);
      int dst_buf_size = bound;
      if (buf_size < dst_buf_size)
        return dst_buf_size;
      if (NULL == dst_buf)
        return -1;
      int ret = compress((unsigned char*) dst_buf, &bound, src_buf,
                         src_buf_size);
      if (ret == Z_OK)
        return bound;
      else
        return -1;
    }
    default:
      return -1;
  }
}

}
