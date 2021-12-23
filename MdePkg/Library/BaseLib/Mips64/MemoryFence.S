#------------------------------------------------------------------------------
#
# MemoryFence() for Mips64
#
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
#------------------------------------------------------------------------------

ASM_GLOBAL ASM_PFX(MemoryFence)

#
# Memory fence for Mips64
#
ASM_PFX(MemoryFence):
  dbar 0
  jirl $zero, $ra, 0
  .end
