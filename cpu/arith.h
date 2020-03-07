#ifndef __ARITH__
#define __ARITH__

#include "util/util.h"
#include <cfenv>
#include <iostream>
using namespace std;

uint64_t amo_amoswap_w_func(const uint64_t &a, const uint64_t &b) { return b; }
uint64_t amo_amoadd_w_func(const uint64_t &a, const uint64_t &b) {
  return a + b;
}
uint64_t amo_amoxor_w_func(const uint64_t &a, const uint64_t &b) {
  return a ^ b;
}
uint64_t amo_amoand_w_func(const uint64_t &a, const uint64_t &b) {
  return a & b;
}
uint64_t amo_amoor_w_func(const uint64_t &a, const uint64_t &b) {
  return a | b;
}
uint64_t amo_amomin_w_func(const uint64_t &a, const uint64_t &b) {
  return (int32_t)a < (int32_t)b ? a : b;
}
uint64_t amo_amomax_w_func(const uint64_t &a, const uint64_t &b) {
  return (int32_t)a < (int32_t)b ? b : a;
}
uint64_t amo_amominu_w_func(const uint64_t &a, const uint64_t &b) {
  return (uint32_t)a < (uint32_t)b ? a : b;
}
uint64_t amo_amomaxu_w_func(const uint64_t &a, const uint64_t &b) {
  return (uint32_t)a < (uint32_t)b ? b : a;
}
uint64_t amo_amoswap_d_func(const uint64_t &a, const uint64_t &b) { return b; }
uint64_t amo_amoadd_d_func(const uint64_t &a, const uint64_t &b) {
  return a + b;
}
uint64_t amo_amoxor_d_func(const uint64_t &a, const uint64_t &b) {
  return a ^ b;
}
uint64_t amo_amoand_d_func(const uint64_t &a, const uint64_t &b) {
  return a & b;
}
uint64_t amo_amoor_d_func(const uint64_t &a, const uint64_t &b) {
  return a | b;
}
uint64_t amo_amomin_d_func(const uint64_t &a, const uint64_t &b) {
  return (int64_t)a < (int64_t)b ? a : b;
}
uint64_t amo_amomax_d_func(const uint64_t &a, const uint64_t &b) {
  return (int64_t)a < (int64_t)b ? b : a;
}
uint64_t amo_amominu_d_func(const uint64_t &a, const uint64_t &b) {
  return a < b ? a : b;
}
uint64_t amo_amomaxu_d_func(const uint64_t &a, const uint64_t &b) {
  return a < b ? b : a;
}

#define F_FLAGS_NX 0x01
#define F_FLAGS_UF 0x02
#define F_FLAGS_OF 0x04
#define F_FLAGS_DZ 0x08
#define F_FLAGS_NV 0x10

#define F_CLASS_NEG_INF (1UL << 0)
#define F_CLASS_NEG_NORM (1UL << 1)
#define F_CLASS_NEG_DENORM (1UL << 2)
#define F_CLASS_NEG_ZERO (1UL << 3)
#define F_CLASS_POS_ZERO (1UL << 4)
#define F_CLASS_POS_DENORM (1UL << 5)
#define F_CLASS_POS_NORM (1UL << 6)
#define F_CLASS_POS_INF (1UL << 7)
#define F_CLASS_SIGNALING (1UL << 8)
#define F_CLASS_QUIET (1UL << 9)

#define F32_FRAC_LEN 23
#define F32_DEFAULT_QUIET_NAN 0x7fc00000
#define F32_DEFAULT_SIGNAL_NAN 0x7f800001
#define F32_NEG(x) ((x) ^ (1 << 31))
#define F32_SIG(x) (((x) >> 31) & 0x1)
#define F32_EXP(x) (((x) >> 23) & 0xff)
#define F32_FRAC(x) ((x) & ((1 << 23) - 1))
#define F32_IS_SINGAL_NAN(x) (((x)&0x7fc00000) == 0x7f800000 && ((x)&0x3fffff))
#define F32_IS_QUIET_NAN(x) (((x)&0x7fc00000) == 0x7fc00000)
#define F32_COMBINE_FLOAT(sig, exp, frac)                                      \
  (((uint32_t)(bool)(sig) << 31 | ((exp)&0xff) << 23) + (frac))

uint64_t f32_classify(const uint64_t &a) {
  bool sig(F32_SIG(a));
  uint8_t exp(F32_EXP(a));
  uint32_t frac(F32_FRAC(a));

  cout << hex << "SIG : " << (int)sig << endl;
  cout << hex << "EXP : " << (int)exp << endl;
  cout << hex << "FRAC : " << (int)frac << endl;

  if (!exp) {
    if (!frac)
      return !sig ? F_CLASS_POS_ZERO : F_CLASS_NEG_ZERO;
    return !sig ? F_CLASS_POS_DENORM : F_CLASS_NEG_DENORM;
  }
  if (exp == 0xff) {
    if (!frac)
      return !sig ? F_CLASS_POS_INF : F_CLASS_NEG_INF;
    if (F32_IS_SINGAL_NAN(a))
      return F_CLASS_SIGNALING;
    return F_CLASS_QUIET;
  }
  return !sig ? F_CLASS_POS_NORM : F_CLASS_NEG_NORM;
}
void clean_fflags_value() { std::feclearexcept(FE_ALL_EXCEPT); }

uint64_t get_fflags_value() {
  uint64_t flags(0);
  if (std::fetestexcept(FE_INEXACT))
    flags |= F_FLAGS_NX;
  if (std::fetestexcept(FE_UNDERFLOW))
    flags |= F_FLAGS_UF;
  if (std::fetestexcept(FE_OVERFLOW))
    flags |= F_FLAGS_OF;
  if (std::fetestexcept(FE_DIVBYZERO))
    flags |= F_FLAGS_DZ;
  if (std::fetestexcept(FE_INVALID))
    flags |= F_FLAGS_NV;
  return flags;
}

union uint32_float32 {
  uint32_t ui;
  float f;
};

uint32_t f32_eq(const uint64_t &a, const uint64_t &b) {
  union uint32_float32 _a;
  union uint32_float32 _b;
  _a.ui = a;
  _b.ui = b;
  return _a.f == _b.f;
}

uint32_t f32_le(const uint64_t &a, const uint64_t &b) {
  union uint32_float32 _a;
  union uint32_float32 _b;
  _a.ui = a;
  _b.ui = b;
  if (F32_IS_QUIET_NAN(_a.ui))
    _a.ui = F32_DEFAULT_SIGNAL_NAN;
  else if (F32_IS_QUIET_NAN(_b.ui))
    _b.ui = F32_DEFAULT_SIGNAL_NAN;
  return _a.f < _b.f || _a.f == _b.f;
}

uint32_t f32_lt(const uint64_t &a, const uint64_t &b) {
  union uint32_float32 _a;
  union uint32_float32 _b;
  _a.ui = a;
  _b.ui = b;
  if (F32_IS_QUIET_NAN(_a.ui))
    _a.ui = F32_DEFAULT_SIGNAL_NAN;
  else if (F32_IS_QUIET_NAN(_b.ui))
    _b.ui = F32_DEFAULT_SIGNAL_NAN;
  return _a.f < _b.f;
}

uint32_t f32_add(const uint64_t &a, const uint64_t &b) {
  union uint32_float32 _a;
  union uint32_float32 _b;
  union uint32_float32 res;
  _a.ui = a;
  _b.ui = b;
  res.f = _a.f + _b.f;
  if ((res.ui & 0x7fffffff) == F32_DEFAULT_QUIET_NAN)
    return F32_DEFAULT_QUIET_NAN;
  return res.ui;
}

uint32_t f32_mul(const uint64_t &a, const uint64_t &b) {
  union uint32_float32 _a;
  union uint32_float32 _b;
  union uint32_float32 res;
  _a.ui = a;
  _b.ui = b;
  res.f = _a.f * _b.f;
  return res.ui;
}

#endif
