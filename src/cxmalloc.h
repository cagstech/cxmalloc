
#include <stdint.h>
#include <stdbool.h>

struct cxmalloc_context {
    uint8_t proc_id;
    size_t max_alloc;
    size_t allocated;
    void* (*malloc)(struct cxmalloc_context *, size_t);
    bool (*free)(struct cxmalloc_context *, void *);
};


void cxmalloc_init(void* (*in_malloc)(), void (*in_free)());
bool cxmalloc_context_create(struct cxmalloc_context *ctx,
                             uint8_t proc_id,
                             size_t proc_max_alloc);
void* cxmalloc_malloc(struct cxmalloc_context *ctx, size_t size);
bool cxmalloc_free(struct cxmalloc_context *ctx, void *ptr);
