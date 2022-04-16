/*
 * THIS  SOFTWARE   CONTAINS  CONFIDENTIAL  INFORMATION
 * AND TRADE SECRETS OF N.L. VERMA / DATUMSOFT SYSTEMS.
 * USE, DISCLOSURE, COPY OR  REPRODUCTION IS PROHIBITED
 * WITHOUT  THE  PRIOR  EXPRESS  WRITTEN  PERMISSION OF
 * NL VERMA / DATUMSOFT SYSTEMS.
 */

#include "dedupe_engine.h"
#include "prototypes.h"

extern U64			hash_table_size;

BOOL dz_same_hash(PHASH hash1, PHASH hash2)
{
	int i;
	
	// Since current hash size is 128 bit so be default
	// looping for 4 count.
	for (i = 0; i < 4; i++) {
		if (hash1->hash[i] != hash2->hash[i]){
			return false;
		}
	}
	return true;
}
EXPORT_SYMBOL(dz_same_hash);

U32 dz_get_hash_index(U32 hash)
{
	hash = (hash & (hash_table_size -1));
	if (hash == 0) {
		LOGD("hash = %u, hash_table_size = %lli\n", hash, hash_table_size); 
	}
	if (hash == 0) {
		hash = 1;
	}
	return hash;
}

VOID test_hash(PVOID data, UINT size)
{
	PPAGE page = dz_write_page_alloc();
	CHAR	*str = AISA_MODULE_AUTHOR;
	PHASH  hash = dz_hash_alloc();
	PHASH  hash2 = dz_hash_alloc();

	memset(hash, 0, sizeof(HASH));
	memset(hash2, 0, sizeof(HASH));

	//Test 1: Write Data onto a Page and provide that as an input i.e. String is written on page
	WRITE_PAGE(page, str, strlen(str));

	data = kmap(page);
	dz_generate_hash(data, strlen(str), hash);
	kunmap(page);
	
	LOG("Input Data : %s\n", str);

	LOG("TEST1 : HASH1: %x, %x, %x, %x\n", hash->hash[0], hash->hash[1], hash->hash[2], hash->hash[3]);
	LOG("TEST1 : HASH1: %llx, %llx\n", hash->hash64[0], hash->hash64[1]);

	//Test 2: Direct Data as input i.e. string is passed without
	memset(hash2, 0, sizeof(HASH));
	dz_generate_hash(str, strlen(str), hash2);

	LOG("TEST2 : HASH1: %x, %x, %x, %x\n", hash2->hash[0], hash2->hash[1], hash2->hash[2], hash2->hash[3]);
	LOG("TEST2 : HASH1: %llx, %llx\n", hash2->hash64[0], hash2->hash64[1]);

	LOG("RESULT : Both hashes are %s", dz_same_hash(hash, hash2) ? "Equal" : "Different");

	dz_hash_free(hash);
	dz_hash_free(hash2);
}

VOID test_hash2(char *str1, char *str2)
{
	PHASH  hash = dz_hash_alloc();
	PHASH  hash2 = dz_hash_alloc();

	memset(hash, 0, sizeof(HASH));
	memset(hash2, 0, sizeof(HASH));

	dz_generate_hash(str1, strlen(str1), hash);
	
	LOG("Input Data : %s\n", str1);
	LOG("Input Data : %s\n", str2);

	LOG("TEST1 : HASH1: %x, %x, %x, %x\n", hash->hash[0], hash->hash[1], hash->hash[2], hash->hash[3]);
	LOG("TEST1 : HASH1: %llx, %llx\n", hash->hash64[0], hash->hash64[1]);

	//Test 2: Direct Data as input i.e. string is passed without
	memset(hash2, 0, sizeof(HASH));
	dz_generate_hash(str2, strlen(str2), hash2);

	LOG("TEST2 : HASH1: %x, %x, %x, %x\n", hash2->hash[0], hash2->hash[1], hash2->hash[2], hash2->hash[3]);
	LOG("TEST2 : HASH1: %llx, %llx\n", hash2->hash64[0], hash2->hash64[1]);

	LOG("RESULT : Both hashes are %s", dz_same_hash(hash, hash2) ? "Equal" : "Different");

	dz_hash_free(hash);
	dz_hash_free(hash2);
}
