#ifndef MAIN_H
#define MAIN_H

#include <stddef.h>
#include <stdbool.h>
#include "hashtable.h"

enum {
	TableCapacity = 20
};

size_t hash_function(const HashTable* table, const char* str);

bool try_create_table(HashTable* table);

bool try_set_entry(
	HashTable* table,
	const char* key,
	size_t key_size,
	const char* value,
	size_t value_size);

bool test_try_get_value(const HashTable* table, const char* key);

void print_table(const HashTable* table);

bool try_free_table(HashTable* table);

#endif // Include guard.
