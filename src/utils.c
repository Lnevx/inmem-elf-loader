#include "utils.h"

void *memcpy(void *dest, const void *source, size_t count) {
    while (count--)
        ((char *)dest)[count] = ((char *)source)[count];
    return dest;
}
