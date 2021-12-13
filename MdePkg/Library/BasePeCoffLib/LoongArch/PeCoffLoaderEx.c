/** @file
  PE/Coff loader for LoongArch PE image

  Copyright (c) 2021, Loongson Technology Corporation Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BasePeCoffLibInternals.h"
#include <Library/BaseLib.h>

/**
  Performs an LoongArch specific relocation fixup and is a no-op on other
  instruction sets.

  @param  Reloc       Pointer to the relocation record.
  @param  Fixup       Pointer to the address to fix up.
  @param  FixupData   Pointer to a buffer to log the fixups.
  @param  Adjust      The offset to adjust the fixup.

  @return Status code.

**/
RETURN_STATUS
PeCoffLoaderRelocateImageEx (
  IN UINT16      *Reloc,
  IN OUT CHAR8   *Fixup,
  IN OUT CHAR8   **FixupData,
  IN UINT64      Adjust
  )
{
  UINT8  RelocType;
  UINT64 Value = 0;
  UINT64 Tmp1 = 0;
  UINT64 Tmp2 = 0;

  RelocType = (*Reloc) >> 12;

  switch (RelocType) {
    case EFI_IMAGE_REL_BASED_LOONGARCH64_MARK_LA:
      Value = (*(UINT32*)Fixup & 0x1ffffe0) << 7 |       /* lu12i.w 20bits from bit5 */
              (*((UINT32*)Fixup + 1) & 0x3ffc00) >> 10;  /* ori  12bits from bit10 */
      Tmp1 = *((UINT32*)Fixup + 2) & 0x1ffffe0;          /* lu32i.d 20bits from bit5 */
      Tmp2 = *((UINT32*)Fixup + 3) & 0x3ffc00;           /* lu52i.d 12bits from bit10 */
      Value = Value | (Tmp1 << 27) | (Tmp2 << 42);
      Value += Adjust;

      *(UINT32*)Fixup = (*(UINT32*)Fixup & ~0x1ffffe0) | (((Value >> 12) & 0xfffff) << 5);
      if (*FixupData != NULL) {
        *FixupData             = ALIGN_POINTER (*FixupData, sizeof (UINT32));
        *(UINT32 *) (*FixupData) = *(UINT32*)Fixup;
        *FixupData             = *FixupData + sizeof (UINT32);
      }

      Fixup += sizeof(UINT32);
      *(UINT32*)Fixup = (*(UINT32*)Fixup & ~0x3ffc00) | ((Value & 0xfff) << 10);
      if (*FixupData != NULL) {
        *FixupData             = ALIGN_POINTER (*FixupData, sizeof (UINT32));
        *(UINT32 *) (*FixupData) = *(UINT32*)Fixup;
        *FixupData             = *FixupData + sizeof (UINT32);
      }

      Fixup += sizeof(UINT32);
      *(UINT32*)Fixup = (*(UINT32*)Fixup & ~0x1ffffe0) | (((Value >> 32) & 0xfffff) << 5);
      if (*FixupData != NULL) {
        *FixupData             = ALIGN_POINTER (*FixupData, sizeof (UINT32));
        *(UINT32 *) (*FixupData) = *(UINT32*)Fixup;
        *FixupData             = *FixupData + sizeof (UINT32);
      }

      Fixup += sizeof(UINT32);
      *(UINT32*)Fixup = (*(UINT32*)Fixup & ~0x3ffc00) | (((Value >> 52) & 0xfff) << 10);
      if (*FixupData != NULL) {
        *FixupData             = ALIGN_POINTER (*FixupData, sizeof (UINT32));
        *(UINT32 *) (*FixupData) = *(UINT32*)Fixup;
        *FixupData             = *FixupData + sizeof (UINT32);
      }
      break;
    default:
      return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}

/**
  Returns TRUE if the machine type of PE/COFF image is supported. Supported
  does not mean the image can be executed it means the PE/COFF loader supports
  loading and relocating of the image type. It's up to the caller to support
  the entry point.

  @param  Machine   Machine type from the PE Header.

  @return TRUE if this PE/COFF loader can load the image

**/
BOOLEAN
PeCoffLoaderImageFormatSupported (
  IN  UINT16  Machine
  )
{
  if ((Machine == IMAGE_FILE_MACHINE_LOONGARCH64) || (Machine ==  IMAGE_FILE_MACHINE_EBC)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Performs an LOONGARCH-based specific re-relocation fixup and is a no-op on other
  instruction sets. This is used to re-relocated the image into the EFI virtual
  space for runtime calls.

  @param  Reloc       The pointer to the relocation record.
  @param  Fixup       The pointer to the address to fix up.
  @param  FixupData   The pointer to a buffer to log the fixups.
  @param  Adjust      The offset to adjust the fixup.

  @return Status code.

**/
RETURN_STATUS
PeHotRelocateImageEx (
  IN UINT16      *Reloc,
  IN OUT CHAR8   *Fixup,
  IN OUT CHAR8   **FixupData,
  IN UINT64      Adjust
  )
{
  /* to check */
  return PeCoffLoaderRelocateImageEx (Reloc, Fixup, FixupData, Adjust);
}
