// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include "mem_list.c"

void *malloc(size_t size)
{
	/* TODO: Implement malloc(). */
	void *ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED) {
        return NULL;
    }
    mem_list_add(ret, size);
    
	return ret;
}

void *calloc(size_t nmemb, size_t size)
{
	/* TODO: Implement calloc(). */
	size_t total_size = nmemb * size;
	void *ret = malloc(total_size);
	if (ret == NULL) {
		return NULL;
	}

	memset(ret, 0, total_size);

	return ret;
}

void free(void *ptr)
{
	/* TODO: Implement free(). */
	mem_list_del(ptr);
}

void *realloc(void *ptr, size_t size)
{
	/* TODO: Implement realloc(). */
	size_t old_size = *((size_t *)ptr);
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        return NULL;
    }

    memcpy(new_ptr, ptr, old_size);
	free(ptr);

    return new_ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	/* TODO: Implement reallocarray(). */
	size_t total_size = nmemb * size;

	return realloc(ptr, total_size);
}
