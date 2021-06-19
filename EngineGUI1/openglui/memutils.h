#pragma once

typedef struct dynamic_memory_s {
	unsigned int size, elemsize, idx;
	void *ptr;
} dynamic_memory_t;

#define MEM_INIT(st, elemsz, startsz) {\
	(st)->size = startsz;\
	if((st)->size <= 0)\
		(st)->size = 1;\
	(st)->idx = NULL;\
	(st)->elemsize = elemsz;\
	(st)->ptr = malloc((st)->elemsize * (st)->size); \
}

#define MEM_PUSH(st, type, data) {\
	if(((st)->idx + 1) == (st)->size) {\
		(st)->size++;\
		(st)->ptr = realloc((st)->ptr, (st)->elemsize * (st)->size);\
	}\
	((type *)(st)->ptr)[(st)->idx] = data;\
	(st)->idx++;\
}

#define MEM_RESET(st) (st)->idx = 0
#define MEM_SIZE(st) (st)->idx
#define MEM_POP(st, type) ((type *)(st)->ptr)[(st)->idx]; (st)->idx--;
#define MEM_GETELEMENT(st, type, idx) ((type *)(st)->ptr)[idx]
#define MEM_SETELEMENT(st, type, idx, newelem) ((type *)(st)->ptr)[idx] = newelem

#define MEM_FREE(st)\
	free((st)->ptr);


