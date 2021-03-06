/* ecc-gost-gc512a.c

   Copyright (C) 2016-2020 Dmitry Eremin-Solenikov

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include <nettle/ecc.h>
#include "ecc-gost-curve.h"
#include "ecc-internal.h"

#define USE_REDC 0

#if defined __clang__ || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
# pragma GCC diagnostic ignored "-Wunused-const-variable"
#endif
#if GMP_NUMB_BITS == 32
#include "ecc/ecc-gost-gc512a-32.h"
#elif GMP_NUMB_BITS == 64
#include "ecc/ecc-gost-gc512a-64.h"
#else
#error unsupported configuration
#endif

static void
ecc_gost_gc512a_modp (const struct ecc_modulo *m, mp_limb_t *rp)
{
  mp_size_t mn = m->size;
  mp_limb_t hi;

  hi = mpn_addmul_1(rp, rp + mn, mn, 0x239);
  hi = sec_add_1 (rp, rp, mn, hi * 0x239);
  hi = sec_add_1 (rp, rp, mn, hi * 0x239);
  assert(hi == 0);
}

#define ecc_gost_gc512a_modp ecc_gost_gc512a_modp
#define ecc_gost_gc512a_modq ecc_mod

const struct ecc_curve _nettle_gost_gc512a =
{
  {
    512,
    ECC_LIMB_SIZE,
    ECC_BMODP_SIZE,
    ECC_REDC_SIZE,
    ECC_MOD_INV_ITCH (ECC_LIMB_SIZE),
    0,

    ecc_p,
    ecc_Bmodp,
    ecc_Bmodp_shifted,
    ecc_redc_ppm1,

    ecc_pp1h,
    ecc_gost_gc512a_modp,
    ecc_gost_gc512a_modp,
    ecc_mod_inv,
    NULL,
  },
  {
    512,
    ECC_LIMB_SIZE,
    ECC_BMODQ_SIZE,
    0,
    ECC_MOD_INV_ITCH (ECC_LIMB_SIZE),
    0,

    ecc_q,
    ecc_Bmodq,
    ecc_Bmodq_shifted,
    NULL,
    ecc_qp1h,

    ecc_gost_gc512a_modq,
    ecc_gost_gc512a_modq,
    ecc_mod_inv,
    NULL,
  },

  USE_REDC,
  ECC_PIPPENGER_K,
  ECC_PIPPENGER_C,

  ECC_ADD_JJA_ITCH (ECC_LIMB_SIZE),
  ECC_ADD_JJJ_ITCH (ECC_LIMB_SIZE),
  ECC_DUP_JJ_ITCH (ECC_LIMB_SIZE),
  ECC_MUL_A_ITCH (ECC_LIMB_SIZE),
  ECC_MUL_G_ITCH (ECC_LIMB_SIZE),
  ECC_J_TO_A_ITCH (ECC_LIMB_SIZE),

  ecc_add_jja,
  ecc_add_jjj,
  ecc_dup_jj,
  ecc_mul_a,
  ecc_mul_g,
  ecc_j_to_a,

  ecc_b,
  ecc_unit,
  ecc_table
};

const struct ecc_curve *nettle_get_gost_gc512a(void)
{
  return &_nettle_gost_gc512a;
}
