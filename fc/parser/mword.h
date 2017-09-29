//  Copyright (c) 2017-2017 The Chaos  Authors. All rights reserved.
//  Created on: 2017.9.19 Author: kerry

#ifndef CHAOS_FC_MWORD_H__
#define CHAOS_FC_MWORD_H__

#pragma once

#include "basic/basictypes.h"

struct MWORD {
  enum {
    MAXBASE = 0x1FFFFFFF,
  };

  int base_ :30;
  unsigned int mul_ :2;

  MWORD() {
  }
  ;

  MWORD(int n);
  MWORD(int base, uint32 mul);
  operator int64() const {
    return GetValue();
  }
  int64 GetValue() const;
  int64 GetABSValue() const;		//绝对值
  MWORD operator-=(const MWORD& d);
  MWORD operator-=(const int64 d);
  MWORD operator+=(const MWORD& d);
  MWORD operator+=(const int64 d);
  MWORD operator*=(const MWORD& d);
  MWORD operator*=(const int64 d);
  MWORD operator/=(const MWORD& d);
  MWORD operator/=(const int64 d);
  int64 operator+(const MWORD& d) const;
  int64 operator-(const MWORD& d) const;
  int64 operator*(const MWORD& d) const;
  int64 operator/(const MWORD& d) const;
  int64 operator+(const int d) const;
  int64 operator-(const int d) const;
  int64 operator*(const int d) const;
  int64 operator/(const int d) const;
  MWORD operator=(const MWORD& d);
  MWORD operator=(const int64 d);
  bool operator==(const MWORD& d) const;
  bool operator==(const int64 d) const;
  bool operator==(const int d) const;
  bool operator!=(const MWORD& d) const;
  bool operator!=(const int64 d) const;
  bool operator!=(const int d) const;
  bool operator>(int d) const;
  bool operator>(MWORD d) const;
  bool operator<(int d) const;
  bool operator<(MWORD d) const;
  bool IsZero() const {
    return base_ == 0;
  }
  bool IsMinus() const {
    return base_ < 0;
  }
  unsigned int GetMul() {
    return mul_;
  }
  unsigned int GetBase() {
    return base_;
  }
  uint32 GetRawData();
  void SetRawData(uint32 dw);
};

#endif
