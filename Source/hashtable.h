#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef enum {
	HashTableResult_Success = 0,
	HashTableResult_UnknownError,
	HashTableResult_NullArgument,
	HashTableResult_OutOfRangeArgument,
	HashTableResult_TableFull,
	HashTableResult_NotImplemented,
	HashTableResult_EntryNotFound
} HashTableResult;

typedef struct {
	void* key;
	void* value;
} HashTableEntry;

typedef struct HashTable HashTable;

typedef size_t (*HashTableHashCallback)(const HashTable*, const void*);

typedef int (*HashTableCompareCallback)(const void*, const void*);

typedef void* (*HashTableCopyCallback)(void*, const void*);

typedef HashTableResult (*HashTableCollisionCallback)(
	const HashTable*,
	const HashTableEntry*);

struct HashTable {
	HashTableHashCallback hash_function;
	HashTableCompareCallback compare_function;
	HashTableCopyCallback copy_function;
	HashTableCollisionCallback collision_function;
	size_t count;
	size_t size;
	HashTableEntry** entries;
};

HashTableResult HashTable_create_entry(
	HashTableEntry* entry,
	const void* key,
	size_t key_size,
	const void* value,
	size_t value_size
);

HashTableResult HashTable_create_table(
	HashTable* table,
	size_t size,
	HashTableHashCallback hash_function,
	HashTableCompareCallback compare_function,
	HashTableCopyCallback copy_function,
	HashTableCollisionCallback collision_function
);

HashTableResult HashTable_free_entry(HashTableEntry* entry);

HashTableResult HashTable_free_entries(HashTable* table);

HashTableResult HashTable_set_entry(
	HashTable* table,
	const void* key,
	size_t key_size,
	const void* value,
	size_t value_size
);

HashTableResult HashTable_try_get_value(
	void* value,
	const HashTable* table,
	const void* key);

HashTableResult HashTable_default_collision(
	const HashTable* table,
	const HashTableEntry* entry);

#endif // Include guard.
