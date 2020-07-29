#include "hashtable.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

HashTableResult HashTable_create_entry(
	HashTableEntry* entry,
	const void* key,
	size_t key_size,
	const void* value,
	size_t value_size
) {
	if (key_size < 1 || value_size < 1)
		return HashTableResult_OutOfRangeArgument;

	entry->key = malloc(key_size);
	entry->value = malloc(value_size);

	memcpy(entry->key, key, key_size);
	memcpy(entry->value, value, value_size);

	return HashTableResult_Success;
}

HashTableResult HashTable_create_table(
	HashTable* table,
	size_t size,
	HashTableHashCallback hash_function,
	HashTableCompareCallback compare_function,
	HashTableCopyCallback copy_function,
	HashTableCollisionCallback collision_function
) {
	if (size < 1)
		return HashTableResult_OutOfRangeArgument;

	if (
		table == NULL ||
		hash_function == NULL ||
		compare_function == NULL ||
		copy_function == NULL
	) {
		return HashTableResult_NullArgument;
	}

	table->hash_function = hash_function;
	table->compare_function = compare_function;
	table->copy_function = copy_function;
	table->collision_function = collision_function;
	table->size = size;
	table->count = 0;
	table->entries = calloc(table->size, sizeof(HashTableEntry*));

	for (
		HashTableEntry** p = table->entries;
		p < table->entries + table->size;
		++p
	) {
		*p = NULL;
	}

	return HashTableResult_Success;
}

HashTableResult HashTable_free_entry(HashTableEntry* entry) {
	if (entry == NULL)
		return HashTableResult_NullArgument;

	free(entry->key);
	free(entry->value);

	return HashTableResult_Success;
}

HashTableResult HashTable_free_entries(HashTable* table) {
	if (table == NULL)
		return HashTableResult_NullArgument;

	for (size_t i = 0; i < table->size; ++i) {
		HashTableEntry* entry = table->entries[i];

		if (entry != NULL)
			HashTable_free_entry(entry);
	}

	free(table->entries);

	return HashTableResult_Success;
}

HashTableResult HashTable_set_entry(
	HashTable* table,
	const void* key,
	size_t key_size,
	const void* value,
	size_t value_size
) {
	if (table == NULL)
		return HashTableResult_NullArgument;

	HashTableEntry* entry = malloc(sizeof(HashTableEntry));
	HashTableResult result = HashTable_create_entry(
		entry,
		key,
		key_size,
		value,
		value_size);

	if (result != HashTableResult_Success)
		return result;

	size_t index = table->hash_function(table, entry->key);
	HashTableEntry* current = table->entries[index];

	if (current == NULL) {
		if (table->count == table->size) {
			HashTable_free_entry(entry);
			free(entry);

			return HashTableResult_TableFull;
		}

		++table->count;
		table->entries[index] = entry;

		return HashTableResult_Success;
	}

	if (table->compare_function(current->key, entry->key) == 0) {
		table->copy_function(current->value, value);

		return HashTableResult_Success;
	}

	return table->collision_function(table, entry);
}

HashTableResult HashTable_try_get_value(
	void** value,
	const HashTable* table,
	const void* key
) {
	if (table == NULL)
		return HashTableResult_NullArgument;

	size_t index = table->hash_function(table, key);
	HashTableEntry* entry = table->entries[index];

	printf("index: %zu\n", index);

	if (entry != NULL && table->compare_function(entry->key, key) == 0) {
		*value = entry->value;

		return HashTableResult_Success;
	}

	return HashTableResult_EntryNotFound;
}

// TODO: Implement collision handling.
HashTableResult HashTable_default_collision(
	const HashTable* table,
	const HashTableEntry* entry
) {
	return HashTableResult_NotImplemented;
}
