/*
 *   File: hash.h
 * Author: Shaun Wetzstein <shaun@us.ibm.com>
 *  Descr: Various int32 hash functions
 *   Note:
 *   Date: 10/03/10
 */

#ifndef __HASH_H__
#define __HASH_H__

#include <stdint.h>

/* ======================================================================= */

static inline int32_t int32_hash1(int32_t);
static inline int32_t int32_hash2(int32_t);
static inline int32_t int32_hash3(int32_t);

typedef uint32_t(*hash_t) (char *, uint32_t);

static inline uint32_t rs_hash(char *, uint32_t);
static inline uint32_t js_hash(char *, uint32_t);
static inline uint32_t pjw_hash(char *, uint32_t);
static inline uint32_t elf_hash(char *, uint32_t);
static inline uint32_t bkdr_hash(char *, uint32_t);
static inline uint32_t sdbm_hash(char *, uint32_t);
static inline uint32_t djb_hash(char *, uint32_t);
static inline uint32_t dek_hash(char *, uint32_t);
static inline uint32_t bp_hash(char *, uint32_t);
static inline uint32_t fnv_hash(char *, uint32_t);
static inline uint32_t ap_hash(char *, uint32_t);

/* ======================================================================= */

static inline int32_t int32_hash1(int32_t key)
{
	key = ~key + (key << 15);
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2057;
	key = key ^ (key >> 16);

	return key;
}

static inline int32_t int32_hash2(int32_t key)
{
	key = (key + 0x7ed55d16) + (key << 12);
	key = (key ^ 0xc761c23c) ^ (key >> 19);
	key = (key + 0x165667b1) + (key << 5);
	key = (key + 0xd3a2646c) ^ (key << 9);
	key = (key + 0xfd7046c5) + (key << 3);
	key = (key ^ 0xb55a4f09) ^ (key >> 16);

	return key;
}

static inline int32_t int32_hash3(int32_t key)
{
	int32_t c2 = 0x27d4eb2d;

	key = (key ^ 61) ^ (key >> 16);
	key = key + (key << 3);
	key = key ^ (key >> 4);
	key = key * c2;
	key = key ^ (key >> 15);

	return key;
}

/* ======================================================================= */

static inline uint32_t rs_hash(char *str, uint32_t len)
{
	uint32_t b = 378551;
	uint32_t a = 63689;
	uint32_t hash = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = hash * a + (*str);
		a = a * b;
	}

	return hash;
}

static inline uint32_t js_hash(char *str, uint32_t len)
{
	uint32_t hash = 1315423911;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash ^= ((hash << 5) + (*str) + (hash >> 2));
	}

	return hash;
}

static inline uint32_t pjw_hash(char *str, uint32_t len)
{
	const uint32_t BitsInUnsignedInt = (uint32_t) (sizeof(uint32_t) * 8);
	const uint32_t ThreeQuarters = (uint32_t) ((BitsInUnsignedInt * 3) / 4);
	const uint32_t OneEighth = (uint32_t) (BitsInUnsignedInt / 8);
	const uint32_t HighBits =
	    (uint32_t) (0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	uint32_t hash = 0;
	uint32_t test = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (hash << OneEighth) + (*str);

		if ((test = hash & HighBits) != 0) {
			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}

	return hash;
}

static inline uint32_t elf_hash(char *str, uint32_t len)
{
	uint32_t hash = 0;
	uint32_t x = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (hash << 4) + (*str);
		if ((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}

	return hash;
}

static inline uint32_t bkdr_hash(char *str, uint32_t len)
{
	uint32_t seed = 131;	/* 31 131 1313 13131 131313 etc.. */
	uint32_t hash = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (hash * seed) + (*str);
	}

	return hash;
}

static inline uint32_t sdbm_hash(char *str, uint32_t len)
{
	uint32_t hash = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

static inline uint32_t djb_hash(char *str, uint32_t len)
{
	uint32_t hash = 5381;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) + hash) + (*str);
	}

	return hash;
}

static inline uint32_t dek_hash(char *str, uint32_t len)
{
	uint32_t hash = len;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = ((hash << 5) ^ (hash >> 27)) ^ (*str);
	}
	return hash;
}

static inline uint32_t bp_hash(char *str, uint32_t len)
{
	uint32_t hash = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash = hash << 7 ^ (*str);
	}

	return hash;
}

static inline uint32_t fnv_hash(char *str, uint32_t len)
{
	const uint32_t fnv_prime = 0x811C9DC5;
	uint32_t hash = 0;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash *= fnv_prime;
		hash ^= (*str);
	}

	return hash;
}

static inline uint32_t ap_hash(char *str, uint32_t len)
{
	uint32_t hash = 0xAAAAAAAA;
	uint32_t i = 0;

	for (i = 0; i < len; str++, i++) {
		hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
		    (~((hash << 11) + ((*str) ^ (hash >> 5))));
	}

	return hash;
}

/* ======================================================================= */

#endif				/* __HASH_H__ */
