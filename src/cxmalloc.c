
#include <stdint.h>
#include <stdbool.h>
#include "cxmalloc.h"


void* (*usr_malloc)(size_t) = NULL;
void (*usr_free)(void*) = NULL;


void cxmalloc_init(void* (*in_malloc)(), void (*in_free)()){
    usr_malloc = in_malloc;
    usr_free = in_free;
}


bool cxmalloc_context_create(struct cxmalloc_context *ctx,
                             uint8_t proc_id,
                             size_t proc_max_alloc){
    if(ctx==NULL || proc_max_alloc == 0) return NULL;
    ctx->proc_id = proc_id;
    ctx->max_alloc = proc_max_alloc;
    ctx->allocated = 0;
    ctx->malloc = cxmalloc_malloc;
    ctx->free = cxmalloc_free;
    return true;
}

#define CXMALLOC_HEADER_SIZE   3
void* cxmalloc_malloc(struct cxmalloc_context *ctx, size_t size){
    if(usr_malloc==NULL) return NULL;
    if(ctx == NULL || size == 0) return NULL;
    if(ctx->allocated + size > ctx->max_alloc) return NULL;
    void *ptr = usr_malloc(size + CXMALLOC_HEADER_SIZE);
    if(ptr==NULL) return NULL;
    ctx->allocated += size + CXMALLOC_HEADER_SIZE;
    *((uint16_t*)ptr) = (uint16_t)size;
    *((uint8_t*)ptr+2) = ctx->proc_id;
    return ptr+CXMALLOC_HEADER_SIZE;
}

bool cxmalloc_free(struct cxmalloc_context *ctx, void *ptr){
    if(usr_free==NULL) return false;
    if(ctx == NULL || ptr == NULL) false;
    uint16_t buflen = *((uint16_t*)ptr-3);
    uint8_t proc_id = *((uint8_t*)ptr-1);
    if(proc_id != ctx->proc_id) return false;
    if(ctx->allocated - buflen < 0) return false;
    ctx->allocated -= buflen;
    usr_free(ptr-CXMALLOC_HEADER_SIZE);
    return true;
}
