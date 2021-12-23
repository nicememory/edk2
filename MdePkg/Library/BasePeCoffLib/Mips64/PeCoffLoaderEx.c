/** @file
  PE/Coff loader for Mips64 PE image
  Copyright (c) 2021, Loongson Technology Corporation Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "BasePeCoffLibInternals.h"
#include <Library/BaseLib.h>

/**
  Performs an Mips64 specific relocation fixup and is a no-op on other
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
  // TODO: To fix it
  switch (RelocType) {
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
  if ((Machine == IMAGE_FILE_MACHINE_MIPS64) || (Machine ==  IMAGE_FILE_MACHINE_EBC)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Performs an Mips64-based specific re-relocation fixup and is a no-op on other
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
