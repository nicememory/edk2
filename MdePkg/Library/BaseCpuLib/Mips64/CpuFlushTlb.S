#------------------------------------------------------------------------------
#
# CpuFlushTlb() for Mips64
#
# Copyright (c) 2021 Jintao Yin <jintao.yin@i-soft.com.cn>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
#------------------------------------------------------------------------------
ASM_GLOBAL ASM_PFX(CpuFlushTlb)

ASM_PFX(CpuFlushTlb):
  tlbflush
  jirl $zero, $ra, 0
  .end
