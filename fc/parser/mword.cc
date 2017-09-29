#include "mword.h"

MWORD::MWORD(int n) {
  mul_ = 0;
  while (n > MAXBASE || n < -MAXBASE) {
    mul_++;
    n /= 16;
  }
  base_ = n;
}

MWORD::MWORD(int base, uint32 mul) {
  base_ = base;
  mul_ = mul;
}

int64 MWORD::GetValue() const {
  int64 n = base_;
  for (uint32 i = 0; i < mul_; i++)
    n *= 16;
  return n;
}

int64 MWORD::GetABSValue() const {
  int64 n = GetValue();
  if (n < 0)
    n = -n;
  return n;
}

MWORD MWORD::operator-=(const MWORD& d) {
  int64 n = d;
  *this -= n;
  return *this;
}

MWORD MWORD::operator-=(const int64 d) {
  int64 n = *this;
  n -= d;
  *this = n;
  return *this;
}

MWORD MWORD::operator+=(const MWORD& d) {
  int64 n = d;
  *this += n;
  return *this;
}

MWORD MWORD::operator+=(const int64 d) {
  int64 n = *this;
  n += d;
  *this = n;
  return *this;
}

MWORD MWORD::operator*=(const MWORD& d) {
  int64 n = d;
  *this *= n;
  return *this;
}

MWORD MWORD::operator*=(const int64 d) {
  int64 n = *this;
  n *= d;
  *this = n;
  return *this;
}

MWORD MWORD::operator/=(const MWORD& d) {
  int64 n = d;
  *this /= n;
  return *this;
}

MWORD MWORD::operator/=(const int64 d) {
  int64 n = *this;
  n /= d;
  *this = n;
  return *this;
}

int64 MWORD::operator+(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n + m;
}

int64 MWORD::operator-(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n - m;
}

int64 MWORD::operator*(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n * m;
}

int64 MWORD::operator/(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n / m;
}

int64 MWORD::operator+(const int d) const {
  int64 n = *this;
  return n + d;
}

int64 MWORD::operator-(const int d) const {
  int64 n = *this;
  return n - d;
}

int64 MWORD::operator*(const int d) const {
  int64 n = *this;
  return n * d;
}

int64 MWORD::operator/(const int d) const {
  int64 n = *this;
  return n / d;
}

MWORD MWORD::operator=(const MWORD& d) {
  mul_ = d.mul_;
  base_ = d.base_;
  return *this;
}

MWORD MWORD::operator=(const int64 n) {
  int64 d = n;
  int inc = 0;
  mul_ = 0;
  while (inc > MAXBASE || inc < -MAXBASE) {
    inc = (d % 16) >= 8;	//四舍五入
    d /= 16;
    mul_++;
    if (mul_ == 3)
      break;
  }
  base_ = (int) (d + inc);
  return *this;
}

bool MWORD::operator==(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n == m;
}

bool MWORD::operator==(const int64 d) const {
  int64 n = *this;
  return n == d;
}

bool MWORD::operator==(const int d) const {
  int64 n = *this;
  return n == d;
}

bool MWORD::operator!=(const MWORD& d) const {
  int64 n = *this;
  int64 m = d;
  return n != m;
}

bool MWORD::operator!=(const int64 d) const {
  int64 n = *this;
  return n != d;
}

bool MWORD::operator!=(const int d) const {
  int64 n = *this;
  return n != d;
}

bool MWORD::operator>(int d) const {
  int64 n = *this;
  return n > d;
}

bool MWORD::operator>(MWORD d) const {
  int64 n = *this;
  int64 m = d;
  return n > m;
}

bool MWORD::operator<(int d) const {
  int64 n = *this;
  return n < d;
}

bool MWORD::operator<(MWORD d) const {
  int64 n = *this;
  int64 m = d;
  return n < m;
}

uint32 MWORD::GetRawData() {
  return *(uint32*) this;
}

void MWORD::SetRawData(uint32 dw) {
  uint32* p = (uint32*) this;
  *p = dw;
}
