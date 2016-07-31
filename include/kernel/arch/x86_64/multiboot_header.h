#pragma once

#include <stdint.h>

typedef struct {
  /* Must be MULTIBOOT_MAGIC - see above.  */
  uint32_t magic;

  /* ISA */
  uint32_t architecture;

  /* Total header length.  */
  uint32_t header_length;

  /* The above fields plus this one must equal 0 mod 2^32. */
  uint32_t checksum;
} MultibootHeader;
