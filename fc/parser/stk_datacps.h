//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_STK_DATACPS_H__
#define CHAOS_FC_STK_DATACPS_H__

#pragma once

#include "his_head.h"
namespace fc_data {

bool IsEqualGUID(const struct GUID rguid, const struct GUID lguid);
int UnCompressData(const unsigned char* src_buf		//	源数据
    , void* dst_buf		//	目标缓冲区
    , unsigned int buf_size		//	目标缓冲区大小
    );

int CompressData(const unsigned char* src_buf  //源数据
    , const unsigned int src_buf_size  //源数据长度
    , HISHD_COMPRESS_TYPE ct_type  //压缩类型
    , void* dst_buf  //目标缓冲区
    , unsigned int buf_size  //目标缓冲区大小
    );
}

#endif
