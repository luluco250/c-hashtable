#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"

int main() {
	const char* key0 = "foo";
	const char* value0 = "bar";
	const char* key1 = "abc";
	const char* value1 = "123";
	HashTable table;

	if (!(
		try_create_table(&table) &&
		try_set_entry(&table, key0, sizeof(key0), value0, sizeof(value0)) &&
		try_set_entry(&table, key1, sizeof(key1), value1, sizeof(value1)) &&
		test_try_get_value(&table, key0) &&
		test_try_get_value(&table, key1))
	) {
		return EXIT_FAILURE;
	}

	print_table(&table);

	return try_free_table(&table) ? EXIT_SUCCESS : EXIT_FAILURE;
}

// TODO: Implement an *actual* hash function.
size_t hash_function(const HashTable* table, const char* str) {
	size_t index = 0;

	for (const char* p = str; *p != '\0'; ++p)
		index += *p;

	return index % table->size;
}

bool try_create_table(HashTable* table) {
	HashTableResult result = HashTable_create_table(
		table,
		TableCapacity,
		hash_function,
		strcmp,
		strcpy,
		HashTable_default_collision);

	if (result == HashTableResult_Success) {
		fputs("Table created!\n", stdout);
		return true;
	}

	fprintf(stderr, "Failed to create table, result code: %i\n", result);

	return false;
}

bool try_set_entry(
	HashTable* table,
	const char* key,
	size_t key_size,
	const char* value,
	size_t value_size
) {
	HashTableResult result = HashTable_set_entry(
		table,
		key,
		key_size,
		value,
		value_size);

	if (result == HashTableResult_Success) {
		fprintf(stdout, "Set \"%s\" to \"%s\"\n", key, value);

		return true;
	}

	fprintf(stderr, "Failed to set \"%s\", result code: %i\n", key, result);

	return false;
}

bool test_try_get_value(const HashTable* table, const char* key) {
	char* value = NULL;

	HashTableResult result = HashTable_try_get_value(&value, table, key);

	switch (result) {
		case HashTableResult_Success: {
			printf("Got value of \"%s\": \"%s\"\n", key, value);

			return true;
		}
		case HashTableResult_EntryNotFound: {
			fprintf(stderr, "Failed to find entry \"%s\"\n", key);

			return false;
		}
	}

	fprintf(
		stderr,
		"Failed to get value of \"%s\", result code: %i\n",
		key,
		result);

	return false;
}

void print_table(const HashTable* table) {
	fputs("Table contents:\n", stdout);

	for (size_t i = 0; i < table->size; ++i) {
		HashTableEntry* entry = table->entries[i];

		if (entry == NULL) {
			printf("  %zu: (null)\n", i);
			continue;
		}

		printf(
			"  %zu: (%s, %s)\n",
			i,
			(const char*)entry->key,
			(const char*)entry->value);
	}

	fputs("--\n", stdout);
}

bool try_free_table(HashTable* table) {
	HashTableResult result = HashTable_free_entries(table);

	if (result == HashTableResult_Success)
		return true;

	fprintf(stderr, "Failed to free table, result code: %i", result);
	return false;
}
