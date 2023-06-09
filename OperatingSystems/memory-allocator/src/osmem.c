// SPDX-License-Identifier: BSD-3-Clause
#include "osmem.h"
#include "helpers.h"

#define ALIGNMENT 8
#define SIZE_BLOCK_META (sizeof(struct block_meta))

static struct block_meta *head_brk_list;
static int is_first_brk = 1;

// coalesce - uneste 2 blocuri consecutive cu STATUS_FREE in unul singur
void coalesce(void)
{
	struct block_meta *current_block = head_brk_list;

	while (current_block && current_block->next) {
		if (current_block->status == STATUS_FREE && current_block->next == STATUS_FREE) {
			current_block->size += current_block->next->size + SIZE_BLOCK_META;
			current_block->next = current_block->next->next;
		} else {
			current_block = current_block->next;
		}
	}
}

// functie ce cauta cel mai potrivit block care este free(de preferat unul fix de
// dimensiunea cautata, iar daca nu exista pe cel cu cea mai buna dimensiune)
struct block_meta *find_best_fit_block(size_t size)
{
	struct block_meta *current_block = head_brk_list;
	struct block_meta *best_fit = NULL;

	while (current_block) {
		if (current_block->status == STATUS_FREE && current_block->size >= size) {
			// daca block ul are fix dimeniunea cautata => il salvez si opresc executia
			if (current_block->size == size) {
				return current_block;
			// daca block ul curent este mai bun decat best fit, dar inca nu perfect => il salvez
			} else {
				if (!best_fit || current_block->size < best_fit->size)
					best_fit = current_block;
			}
		}
		// iteratie
		current_block = current_block->next;
	}

	return best_fit;
}

// imparte block ul primit in unul cu STATUS_FREE si unul cu STATUS_ALLOC si dimenisunea ceruta
struct block_meta *split_block(struct block_meta *current_block, size_t block_size)
{
	struct block_meta *free_block = (struct block_meta *)((char *)current_block + block_size);

	free_block->size = current_block->size - block_size;
	free_block->status = STATUS_FREE;
	free_block->next = current_block->next;

	current_block->size = block_size - SIZE_BLOCK_META;
	current_block->status = STATUS_ALLOC;
	current_block->next = free_block;

	return current_block;
}

// functie auxiliara apelata atat in malloc, cat si in calloc
void *os_malloc_aux(size_t size, size_t threshold)
{
	// alinierea alocarii memoriei la 8 bytes
	if (size % ALIGNMENT != 0)
		size += ALIGNMENT - (size % ALIGNMENT);

	// dimensiunea unui block intreg
	size_t block_size = size + SIZE_BLOCK_META;

	// daca block_size < threshold(128 * 1024 pentru malloc si 4 * 1024 pentru calloc)
	// => incercare gasire block free/alocare brk()
	if (block_size < threshold) {
		coalesce();
		struct block_meta *current_block = find_best_fit_block(size);

		// daca current_block este NULL => nu am gasit niciun block cu STATUS_FREE => alocare cu brk()
		if (current_block == NULL) {
			struct block_meta *block_brk = NULL;
			// daca este prima alocare cu brk => sbrk(0x20000); altfel => sbrk(block_size)
			if (is_first_brk == 1) {
				block_brk = sbrk(128 * 1024);
				is_first_brk = 0;
			} else {
				block_brk = sbrk(block_size);
			}

			if (block_brk == (void *)-1)
				return NULL;

			current_block = block_brk;
			current_block->size = size;
			current_block->status = STATUS_ALLOC;
			current_block->next = NULL;
		// split - din block ul gasit, cu dimensiune mai mare decat este necesar, o sa fac 2:
		// unul pe care il folosesc si il marchez cu STATUS_ALLOC si cel nefolosit cu STATUS_FREE
		} else {
			if (current_block->size > block_size)
				current_block = split_block(current_block, block_size);
			// reuse - block ul gasit are fix dimensiunea cautata => ii modific doar statusul in STATUS_ALLOC
			else
				current_block->status = STATUS_ALLOC;
		}

		return (void *)(current_block + 1);
	// altfel => aloc memorie cu mmap
	} else {
		struct block_meta *block_mmap = mmap(NULL, block_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

		if (block_mmap == MAP_FAILED)
			return NULL;

		block_mmap->size = size;
		block_mmap->status = STATUS_MAPPED;
		block_mmap->next = NULL;

		return (void *)(block_mmap + 1);
	}

	return NULL;
}

void *os_malloc(size_t size)
{
	if (size == 0)
		return NULL;

	// apel os_malloc_aux cu threshold = 128 * 1024
	void *ret = os_malloc_aux(size, 128 * 1024);

	return ret;
}

void os_free(void *ptr)
{
	if (ptr == NULL)
		return;

	// pointer la block ul ce urmeaza sa fie STATUS_FREE
	struct block_meta *block = (struct block_meta *)ptr - 1;

	// free in cazul alocarii cu brk
	if (block->status == STATUS_ALLOC) {
		block->status = STATUS_FREE;
		head_brk_list = block;
	//free in cazul alocarii cu mmap
	} else {
		if (block->status == STATUS_MAPPED)
			munmap(block, block->size + SIZE_BLOCK_META);
	}
}

void *os_calloc(size_t nmemb, size_t size)
{
	if (nmemb == 0 || size == 0)
		return NULL;

	size_t total_size = nmemb * size;

	// apel os_malloc_aux cu threshold = 4 * 1024
	void *ret = os_malloc_aux(total_size, 4 * 1024);

	if (ret != NULL)
		memset(ret, 0, total_size);

	return ret;
}

void *os_realloc(void *ptr, size_t size)
{
	if (ptr == NULL)
		return os_malloc(size);

	if (size == 0) {
		os_free(ptr);
		return NULL;
	}

	struct block_meta *block = (struct block_meta *)ptr - 1;

	// block ul curent are dimensiunea suficienta
	if (block->size >= size)
		return ptr;
	// aloc un block nou si copiez datele in el
	else {
		void *new_ptr = os_malloc(size);

		if (new_ptr != NULL) {
			memcpy(new_ptr, ptr, block->size);
			os_free(ptr);
		}

		return new_ptr;
	}
}
