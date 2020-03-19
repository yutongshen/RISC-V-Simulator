#ifndef __ARITH__
#define __ARITH__

#include <math.h>
#include <cfenv>
#include <iostream>
#include "util/util.h"
using namespace std;

uint64_t mulhu(const uint64_t &a, const uint64_t &b)
{
    uint64_t a0(a & -1U);
    uint64_t b0(b & -1U);
    uint64_t a1(a >> 32);
    uint64_t b1(b >> 32);
    uint64_t t0, t1, t2, t3, res;

    t0 = a0 * b0;  //   **
    t1 = a1 * b0;  //  **
    t2 = a0 * b1;  //  **
    t3 = a1 * b1;  // **

    res = (t0 >> 32) + (t1 & -1U) + (t2 & -1U);
    res = res >> 32;
    res += (t1 >> 32) + (t2 >> 32) + t3;

    return res;
}

int64_t mulh(const int64_t &a, const int64_t &b)
{
    uint64_t mask((a >> 63) ^ (b >> 63));
    uint64_t abs_a((a ^ (a >> 63)) - (a >> 63));
    uint64_t abs_b((b ^ (b >> 63)) - (b >> 63));
    return (mulhu(abs_a, abs_b) ^ mask) - mask * (a * b == 0);
}

int64_t mulhsu(const int64_t &a, const uint64_t &b)
{
    uint64_t mask(a >> 63);
    uint64_t abs_a((a ^ mask) - mask);
    return (mulhu(abs_a, b) ^ mask) - mask * (a * b == 0);
}

uint64_t amo_amoswap_w_func(const uint64_t &a, const uint64_t &b)
{
    return b;
}
uint64_t amo_amoadd_w_func(const uint64_t &a, const uint64_t &b)
{
    return a + b;
}
uint64_t amo_amoxor_w_func(const uint64_t &a, const uint64_t &b)
{
    return a ^ b;
}
uint64_t amo_amoand_w_func(const uint64_t &a, const uint64_t &b)
{
    return a & b;
}
uint64_t amo_amoor_w_func(const uint64_t &a, const uint64_t &b)
{
    return a | b;
}
uint64_t amo_amomin_w_func(const uint64_t &a, const uint64_t &b)
{
    return (int32_t) a < (int32_t) b ? a : b;
}
uint64_t amo_amomax_w_func(const uint64_t &a, const uint64_t &b)
{
    return (int32_t) a < (int32_t) b ? b : a;
}
uint64_t amo_amominu_w_func(const uint64_t &a, const uint64_t &b)
{
    return (uint32_t) a < (uint32_t) b ? a : b;
}
uint64_t amo_amomaxu_w_func(const uint64_t &a, const uint64_t &b)
{
    return (uint32_t) a < (uint32_t) b ? b : a;
}
uint64_t amo_amoswap_d_func(const uint64_t &a, const uint64_t &b)
{
    return b;
}
uint64_t amo_amoadd_d_func(const uint64_t &a, const uint64_t &b)
{
    return a + b;
}
uint64_t amo_amoxor_d_func(const uint64_t &a, const uint64_t &b)
{
    return a ^ b;
}
uint64_t amo_amoand_d_func(const uint64_t &a, const uint64_t &b)
{
    return a & b;
}
uint64_t amo_amoor_d_func(const uint64_t &a, const uint64_t &b)
{
    return a | b;
}
uint64_t amo_amomin_d_func(const uint64_t &a, const uint64_t &b)
{
    return (int64_t) a < (int64_t) b ? a : b;
}
uint64_t amo_amomax_d_func(const uint64_t &a, const uint64_t &b)
{
    return (int64_t) a < (int64_t) b ? b : a;
}
uint64_t amo_amominu_d_func(const uint64_t &a, const uint64_t &b)
{
    return a < b ? a : b;
}
uint64_t amo_amomaxu_d_func(const uint64_t &a, const uint64_t &b)
{
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
#define F32_IS_NAN(x) (((x) &0x7f800000) == 0x7f800000 && ((x) &0x7fffff))
#define F32_IS_SIGNAL_NAN(x) \
    (((x) &0x7fc00000) == 0x7f800000 && ((x) &0x3fffff))
#define F32_IS_QUIET_NAN(x) (((x) &0x7fc00000) == 0x7fc00000)
#define F32_IS_VALID(x) (((x) &0xffffffff00000000UL) == 0xffffffff00000000UL)

#define F64_FRAC_LEN 52
#define F64_DEFAULT_QUIET_NAN 0x7ff8000000000000UL
#define F64_DEFAULT_SIGNAL_NAN 0x7ff0000000000001UL
#define F64_NEG(x) ((x) ^ (1UL << 63))
#define F64_SIG(x) (((x) >> 63) & 0x1)
#define F64_EXP(x) (((x) >> 52) & 0x7ff)
#define F64_FRAC(x) ((x) & ((1UL << 52) - 1))
#define F64_IS_NAN(x)                                       \
    (((x) &0x7ff0000000000000UL) == 0x7ff0000000000000UL && \
     ((x) &0xfffffffffffffUL))
#define F64_IS_SIGNAL_NAN(x)                                \
    (((x) &0x7ff8000000000000UL) == 0x7ff0000000000000UL && \
     ((x) &0x7ffffffffffffUL))
#define F64_IS_QUIET_NAN(x) \
    (((x) &0x7ff8000000000000UL) == 0x7ff8000000000000UL)

uint64_t f32_classify(const uint64_t &a)
{
    bool sig(F32_SIG(a));
    uint8_t exp(F32_EXP(a));
    uint32_t frac(F32_FRAC(a));

    if (!exp) {
        if (!frac)
            return !sig ? F_CLASS_POS_ZERO : F_CLASS_NEG_ZERO;
        return !sig ? F_CLASS_POS_DENORM : F_CLASS_NEG_DENORM;
    }
    if (exp == 0xff) {
        if (!frac)
            return !sig ? F_CLASS_POS_INF : F_CLASS_NEG_INF;
        if (F32_IS_SIGNAL_NAN(a))
            return F_CLASS_SIGNALING;
        return F_CLASS_QUIET;
    }
    return !sig ? F_CLASS_POS_NORM : F_CLASS_NEG_NORM;
}

uint64_t f64_classify(const uint64_t &a)
{
    bool sig(F64_SIG(a));
    uint16_t exp(F64_EXP(a));
    uint64_t frac(F64_FRAC(a));

    if (!exp) {
        if (!frac)
            return !sig ? F_CLASS_POS_ZERO : F_CLASS_NEG_ZERO;
        return !sig ? F_CLASS_POS_DENORM : F_CLASS_NEG_DENORM;
    }
    if (exp == 0x7ff) {
        if (!frac)
            return !sig ? F_CLASS_POS_INF : F_CLASS_NEG_INF;
        if (F64_IS_SIGNAL_NAN(a))
            return F_CLASS_SIGNALING;
        return F_CLASS_QUIET;
    }
    return !sig ? F_CLASS_POS_NORM : F_CLASS_NEG_NORM;
}

void clean_fflags_value()
{
    std::feclearexcept(FE_ALL_EXCEPT);
}

uint64_t get_fflags_value()
{
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
    int32_t i;
    float f;
};

union uint64_float64 {
    uint64_t ui;
    double f;
};

uint32_t f32_eq(const uint64_t &a, const uint64_t &b)
{
    if (!F32_IS_VALID(a) || !F32_IS_VALID(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(a)) {
        if (F32_IS_SIGNAL_NAN(a))
            std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(b)) {
        if (F32_IS_SIGNAL_NAN(b))
            std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint32_float32 _a;
    union uint32_float32 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f == _b.f;
}

uint64_t f64_eq(const uint64_t &a, const uint64_t &b)
{
    if (F64_IS_NAN(a)) {
        if (F64_IS_SIGNAL_NAN(a))
            std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F64_IS_NAN(b)) {
        if (F64_IS_SIGNAL_NAN(b))
            std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint64_float64 _a;
    union uint64_float64 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f == _b.f;
}

uint32_t f32_le(const uint64_t &a, const uint64_t &b)
{
    if (!F32_IS_VALID(a) || !F32_IS_VALID(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(a)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint32_float32 _a;
    union uint32_float32 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f <= _b.f;
}

uint64_t f64_le(const uint64_t &a, const uint64_t &b)
{
    if (F64_IS_NAN(a)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F64_IS_NAN(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint64_float64 _a;
    union uint64_float64 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f <= _b.f;
}

uint32_t f32_lt(const uint64_t &a, const uint64_t &b)
{
    if (!F32_IS_VALID(a) || !F32_IS_VALID(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(a)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F32_IS_NAN(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint32_float32 _a;
    union uint32_float32 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f < _b.f;
}

uint64_t f64_lt(const uint64_t &a, const uint64_t &b)
{
    if (F64_IS_NAN(a)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    if (F64_IS_NAN(b)) {
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    union uint64_float64 _a;
    union uint64_float64 _b;
    _a.ui = a;
    _b.ui = b;
    return _a.f < _b.f;
}

uint32_t f32_min(const uint64_t &a, const uint64_t &b)
{
    if (!(a & (-1U >> 1)) && !(b & (-1U >> 1)))
        return !F32_SIG(a) ? b : a;
    if (F32_IS_SIGNAL_NAN(a) || F32_IS_SIGNAL_NAN(b))
        std::feraiseexcept(FE_INVALID);
    if (F32_IS_NAN(a) && F32_IS_NAN(b))
        return F32_DEFAULT_QUIET_NAN;
    if (F32_IS_NAN(a))
        return b;
    if (F32_IS_NAN(b))
        return a;

    union uint32_float32 _a;
    union uint32_float32 _b;
    _a.ui = a;
    _b.ui = b;

    return (_a.f < _b.f) ? a : b;
}

uint64_t f64_min(const uint64_t &a, const uint64_t &b)
{
    if (!(a & (-1UL >> 1)) && !(b & (-1UL >> 1)))
        return !F64_SIG(a) ? b : a;
    if (F64_IS_SIGNAL_NAN(a) || F64_IS_SIGNAL_NAN(b))
        std::feraiseexcept(FE_INVALID);
    if (F64_IS_NAN(a) && F64_IS_NAN(b))
        return F64_DEFAULT_QUIET_NAN;
    if (F64_IS_NAN(a))
        return b;
    if (F64_IS_NAN(b))
        return a;

    union uint64_float64 _a;
    union uint64_float64 _b;
    _a.ui = a;
    _b.ui = b;

    return (_a.f < _b.f) ? a : b;
}

uint32_t f32_max(const uint64_t &a, const uint64_t &b)
{
    if (!(a & (-1U >> 1)) && !(b & (-1U >> 1)))
        return !F32_SIG(a) ? a : b;
    if (F32_IS_SIGNAL_NAN(a) || F32_IS_SIGNAL_NAN(b))
        std::feraiseexcept(FE_INVALID);
    if (F32_IS_NAN(a) && F32_IS_NAN(b))
        return F32_DEFAULT_QUIET_NAN;
    if (F32_IS_NAN(a))
        return b;
    if (F32_IS_NAN(b))
        return a;

    union uint32_float32 _a;
    union uint32_float32 _b;
    _a.ui = a;
    _b.ui = b;

    return (_a.f < _b.f) ? b : a;
}

uint64_t f64_max(const uint64_t &a, const uint64_t &b)
{
    if (!(a & (-1UL >> 1)) && !(b & (-1UL >> 1)))
        return !F64_SIG(a) ? a : b;
    if (F64_IS_SIGNAL_NAN(a) || F64_IS_SIGNAL_NAN(b))
        std::feraiseexcept(FE_INVALID);
    if (F64_IS_NAN(a) && F64_IS_NAN(b))
        return F64_DEFAULT_QUIET_NAN;
    if (F64_IS_NAN(a))
        return b;
    if (F64_IS_NAN(b))
        return a;

    union uint64_float64 _a;
    union uint64_float64 _b;
    _a.ui = a;
    _b.ui = b;

    return (_a.f < _b.f) ? b : a;
}

uint32_t f32_add(const uint64_t &a, const uint64_t &b)
{
    union uint32_float32 _a;
    union uint32_float32 _b;
    union uint32_float32 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f + _b.f;
    if (F32_IS_QUIET_NAN(res.ui))
        return F32_DEFAULT_QUIET_NAN;
    return res.ui;
}

uint64_t f64_add(const uint64_t &a, const uint64_t &b)
{
    union uint64_float64 _a;
    union uint64_float64 _b;
    union uint64_float64 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f + _b.f;
    if (F64_IS_QUIET_NAN(res.ui))
        return F64_DEFAULT_QUIET_NAN;
    return res.ui;
}

uint32_t f32_mul(const uint64_t &a, const uint64_t &b)
{
    union uint32_float32 _a;
    union uint32_float32 _b;
    union uint32_float32 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f * _b.f;
    return res.ui;
}

uint64_t f64_mul(const uint64_t &a, const uint64_t &b)
{
    union uint64_float64 _a;
    union uint64_float64 _b;
    union uint64_float64 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f * _b.f;
    return res.ui;
}

uint32_t f32_div(const uint64_t &a, const uint64_t &b)
{
    union uint32_float32 _a;
    union uint32_float32 _b;
    union uint32_float32 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f / _b.f;
    return res.ui;
}

uint64_t f64_div(const uint64_t &a, const uint64_t &b)
{
    union uint64_float64 _a;
    union uint64_float64 _b;
    union uint64_float64 res;
    _a.ui = a;
    _b.ui = b;
    res.f = _a.f / _b.f;
    return res.ui;
}

uint32_t f32_sqrt(const uint64_t &a)
{
    union uint32_float32 _a;
    union uint32_float32 res;
    _a.ui = a;
    res.f = sqrtf(_a.f);
    if (F32_IS_QUIET_NAN(res.ui))
        return F32_DEFAULT_QUIET_NAN;
    return res.ui;
}

uint64_t f64_sqrt(const uint64_t &a)
{
    union uint64_float64 _a;
    union uint64_float64 res;
    _a.ui = a;
    res.f = sqrt(_a.f);
    if (F64_IS_QUIET_NAN(res.ui))
        return F64_DEFAULT_QUIET_NAN;
    return res.ui;
}

uint32_t f64_cvt_s_d(const int64_t &a)
{
    if (F64_IS_QUIET_NAN(a))
        return F32_DEFAULT_QUIET_NAN;
    union uint64_float64 _a;
    union uint32_float32 res;
    _a.ui = a;
    res.f = _a.f;
    return res.ui;
}

uint64_t f64_cvt_d_s(const int32_t &a)
{
    if (F32_IS_QUIET_NAN(a))
        return F64_DEFAULT_QUIET_NAN;
    union uint32_float32 _a;
    union uint64_float64 res;
    _a.ui = a;
    res.f = _a.f;
    return res.ui;
}

uint32_t f32_cvt_s_w(const int32_t &a)
{
    union uint32_float32 res;
    res.f = a;
    return res.ui;
}

uint64_t f64_cvt_d_w(const int32_t &a)
{
    union uint64_float64 res;
    res.f = a;
    return res.ui;
}

uint32_t f32_cvt_s_wu(const uint32_t &a)
{
    union uint32_float32 res;
    res.f = a;
    return res.ui;
}

uint64_t f64_cvt_d_wu(const uint32_t &a)
{
    union uint64_float64 res;
    res.f = a;
    return res.ui;
}

uint32_t f32_cvt_s_l(const int64_t &a)
{
    union uint32_float32 res;
    res.f = a;
    return res.ui;
}

uint64_t f64_cvt_d_l(const int64_t &a)
{
    union uint64_float64 res;
    res.f = a;
    return res.ui;
}

uint32_t f32_cvt_s_lu(const uint64_t &a)
{
    union uint32_float32 res;
    res.f = a;
    return res.ui;
}

uint64_t f64_cvt_d_lu(const uint64_t &a)
{
    union uint64_float64 res;
    res.f = a;
    return res.ui;
}

int32_t f32_cvt_w_s(const uint64_t &a)
{
    union uint32_float32 res;
    res.ui = a;
    res.i = res.f;
    if (res.i == (1 << 31) && !F32_SIG(a))
        return ~res.i;
    if (F32_EXP(a) == 0xff) {
        if (F32_FRAC(a))
            return -1U >> 1;
        return !F32_SIG(a) ? (-1U >> 1) : (1U << 31);
    }
    return res.i;
}

int32_t f64_cvt_w_d(const uint64_t &a)
{
    union uint64_float64 _a;
    int32_t res;
    _a.ui = a;
    res = _a.f;
    if (res == (1 << 31) && !F64_SIG(a))
        return ~res;
    if (F64_EXP(a) == 0x7ff) {
        if (F64_FRAC(a))
            return -1U >> 1;
        return !F64_SIG(a) ? (-1U >> 1) : (1U << 31);
    }
    return res;
}

int32_t f32_cvt_wu_s(const uint64_t &a)
{
    union uint32_float32 res;
    res.ui = a;
    res.ui = res.f;
    if (F32_EXP(a) == 0xff) {
        if (F32_FRAC(a))
            return -1U;
        return !F32_SIG(a) ? -1U : 0U;
    }
    if (F32_SIG(a) && res.ui) {
        std::feclearexcept(FE_ALL_EXCEPT);
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    return res.ui;
}

int32_t f64_cvt_wu_d(const uint64_t &a)
{
    union uint64_float64 _a;
    uint32_t res;
    _a.ui = a;
    res = _a.f;
    if (F64_EXP(a) == 0x7ff) {
        if (F64_FRAC(a))
            return -1U;
        return !F64_SIG(a) ? -1U : 0U;
    }
    if (F64_SIG(a) && res) {
        std::feclearexcept(FE_ALL_EXCEPT);
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    return res;
}

int64_t f32_cvt_l_s(const uint64_t &a)
{
    union uint32_float32 res;
    res.ui = a;
    if (F32_EXP(a) == 0xff) {
        if (F32_FRAC(a))
            return -1UL >> 1;
        return !F32_SIG(a) ? (-1UL >> 1) : (1UL << 63);
    }
    return res.f;
}

int64_t f64_cvt_l_d(const uint64_t &a)
{
    union uint64_float64 _a;
    int64_t res;
    _a.ui = a;
    res = _a.f;
    if (res == (1UL << 63) && !F64_SIG(a))
        return ~res;
    if (F64_EXP(a) == 0x7ff) {
        if (F64_FRAC(a))
            return -1UL >> 1;
        return !F64_SIG(a) ? (-1UL >> 1) : (1UL << 63);
    }
    return res;
}

uint64_t f32_cvt_lu_s(const uint64_t &a)
{
    uint64_t _res;
    union uint32_float32 res;
    res.ui = a;
    _res = res.f;
    if (F32_EXP(a) == 0xff) {
        if (F32_FRAC(a))
            return -1UL;
        return !F32_SIG(a) ? -1UL : 0UL;
    }
    if (F32_SIG(a) && _res) {
        std::feclearexcept(FE_ALL_EXCEPT);
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    return _res;
}

uint64_t f64_cvt_lu_d(const uint64_t &a)
{
    union uint64_float64 _a;
    uint64_t res;
    _a.ui = a;
    res = _a.f;
    if (F64_EXP(a) == 0x7ff) {
        if (F64_FRAC(a))
            return -1UL;
        return !F64_SIG(a) ? -1UL : 0UL;
    }
    if (F64_SIG(a) && res) {
        std::feclearexcept(FE_ALL_EXCEPT);
        std::feraiseexcept(FE_INVALID);
        return 0;
    }
    return res;
}

#endif
