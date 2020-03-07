#ifndef __ARITH__
#define __ARITH__

#include "util/util.h"
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


#define F_FLAGS_NV 0x10
#define F_FLAGS_DZ 0x08
#define F_FLAGS_OF 0x04
#define F_FLAGS_UF 0x02
#define F_FLAGS_NX 0x01

#define F32_FRAC_LEN 23
#define F32_DEFAULT_NAN 0x7fc00000
#define F32_NEG(x) ((x) ^ (1 << 31))
#define F32_SIG(x) (((x) >> 31) & 0x1)
#define F32_EXP(x) (((x) >> 23) & 0xff)
#define F32_FRAC(x) ((x) & ((1 << 23) - 1))
#define F32_IS_SINGALING(x) (((x) & 0x7fc00000) == 0x7f800000 && ((x) & 0x3fffff))
#define F32_COMBINE_FLOAT(sig, exp, frac) (((uint32_t)(bool)(sig) << 31 | ((exp) & 0xff) << 23) + (frac))

uint32_t f32_normalize(int32_t num, const int16_t exp, uint8_t revers, uint8_t &flags) {
  bool sig_res(0);
  if (num < 0) {
    sig_res = 1;
    num = -num;
  }
  int8_t res_bits(64 - leading_zero(num));
  int8_t shift(res_bits - F32_FRAC_LEN - revers - 1);

  cout << hex << "SHIFT : " << (int)shift << endl;
  cout << hex << "EXP : " << exp << endl;
  cout << hex << "NUM_RES : " << num << endl;

  if (exp + shift >= 0xff) return F32_COMBINE_FLOAT(sig_res, 0xff, 0);
  if (exp + shift < 0 || !num) {
    if (num) flags |= F_FLAGS_NX;
    return F32_COMBINE_FLOAT(sig_res, 0, num << exp >> revers);
  }
  if (exp + shift == 0) return F32_COMBINE_FLOAT(sig_res, 0, num << -shift >> revers);

  uint32_t frac_res;
  bool carry(0);
  if (shift >= 0) {
      carry = (num & (1 << (shift + revers - 1))) &&
          ((num & (1 << (shift + revers))) || (num & ((1 << (shift + revers - 1)) - 1)));
      cout << hex << "CARRY : " << carry << endl;
      frac_res = num >> shift >> revers;
      if (num & ((1 << (shift + revers)) - 1)) flags |= F_FLAGS_NX;
      frac_res += carry;
  }
  else {
      frac_res = num << -shift >> revers;
  }
  frac_res &= 0x7fffff;
  return F32_COMBINE_FLOAT(sig_res, exp + shift, frac_res);
}

uint32_t f32_add(const uint64_t &a, const uint64_t &b, uint8_t &flags) {
  uint8_t revers(3);
  bool sig_a(F32_SIG(a));
  bool sig_b(F32_SIG(b));
  bool sig_res(0);
  int16_t exp_a(F32_EXP(a));
  int16_t exp_b(F32_EXP(b));
  int16_t exp_res(exp_a > exp_b ? exp_a : exp_b);
  uint32_t frac_a(F32_FRAC(a));
  uint32_t frac_b(F32_FRAC(b));
  uint8_t exp_diff(exp_a - exp_b);
  bool sig_ab(sig_a ^ sig_b);
  int16_t shift_a;
  int16_t shift_b;
  bool sticky(0);
  bool is_inf_a(0);
  bool is_inf_b(0);
  int32_t num_a(0);
  int32_t num_b(0);

  if (!exp_a) {
    num_a = frac_a << revers;
  }
  else if (exp_a == 0xff) {
    // nan
    if (frac_a) {
      if (F32_IS_SINGALING(a))
        flags |= F_FLAGS_NV;
      return F32_DEFAULT_NAN;
    }
    // infinite
    is_inf_a = 1;
  }
  else {
    num_a = (0x800000 | frac_a) << revers;
  }
  if (sig_a) num_a = -num_a;

  shift_a = exp_res - exp_a;
  if (shift_a) {
    if (shift_a < 31) {
      sticky = num_a & ((1 << shift_a) - 1);
      num_a >>= shift_a;
    }
    else {
      sticky = num_a;
      num_a >>= 31;
    }
  }

  if (!exp_b) {
    num_b = frac_b << revers;
  }
  else if (exp_b == 0xff) {
    // nan
    if (frac_b) {
      if (F32_IS_SINGALING(b))
        flags |= F_FLAGS_NV;
      return F32_DEFAULT_NAN;
    }
    // infinite
    is_inf_b = 1;
  }
  else {
    num_b = (0x800000 | frac_b) << revers;
  }
  if (sig_b) num_b = -num_b;

  shift_b = exp_res - exp_b;
  if (shift_b) {
    if (shift_b < 31) {
      sticky = num_b & ((1 << shift_b) - 1);
      num_b >>= shift_b;
    }
    else {
      sticky = num_b;
      num_b >>= 31;
    }
  }


  cout << endl;

  cout << hex << "EXP_A : " << exp_a << endl;
  cout << hex << "EXP_B : " << exp_b << endl;

  cout << hex << "NUM_A : " << num_a << endl;
  cout << hex << "NUM_B : " << num_b << endl;
  cout << hex << "STICKY : " << sticky << endl;

  if (is_inf_a | is_inf_b) return  is_inf_a && !is_inf_b ? a :
                                  !is_inf_a &&  is_inf_b ? b :
                                     !(num_a ^ num_b) ? a :
                                  ((flags |= F_FLAGS_NV), F32_DEFAULT_NAN);

  return f32_normalize((num_a + num_b) | sticky, exp_res, revers, flags);




  // // Initialize flags
  // flags = 0;

  // if (!exp_diff) {
  //   // Denormalized
  //   if (!exp_a) {
  //     num_a = sig_a ? -frac_a : frac_a;
  //     num_b = sig_b ? -frac_b : frac_b;
  //     num_res = num_a + num_b;
  //     return (num_res >= 0) ? num_res : F32_NEG(-num_res);
  //   }
  //   if (exp_a == 0xff) {
  //     // a or b is nan
  //     if (frac_a | frac_b) {
  //       if (F32_IS_SINGALING(a) || F32_IS_SINGALING(b))
  //         flags |= F_FLAGS_NV;
  //       return F32_DEFAULT_NAN;
  //     }
  //     // a and b are infinite 
  //     if (sig_ab) {
  //       flags |= F_FLAGS_NV;
  //       return F32_DEFAULT_NAN;
  //     }
  //     return a;
  //   }
  //   // Normalized
  //   num_a = frac_a | 0x800000;
  //   num_b = frac_b | 0x800000;
  //   if (sig_a) num_a = -num_a;
  //   if (sig_b) num_b = -num_b;
  //   num_res = num_a + num_b;
  //   if (num_res < 0) {
  //     sig_res = 1;
  //     num_res = -num_res;
  //   }

  //   

  // }
  // float res(*((float *)&a) + *((float *)&b));
  // return *(int32_t *)&res;
}

#endif
