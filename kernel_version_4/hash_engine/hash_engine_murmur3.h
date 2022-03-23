#ifndef _MURMURHASH3_H_
#define _MURMURHASH3_H_

/*
 * MurmurHash3 was written by Austin Appleby, and is placed in the public
 * domain. The author hereby disclaims copyright to this source code.
 */

#include <linux/types.h>
#include <linux/module.h>

void MurmurHash3_x86_32(const void *key, int len, uint32_t seed, void *out);

void MurmurHash3_x86_128(const void *key, int len, uint32_t seed, void *out);

void MurmurHash3_x64_128(const void *key, size_t len, uint32_t seed, void *out);

#endif
