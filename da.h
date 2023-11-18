#include <stddef.h>
#include <stdlib.h>

typedef struct {
    size_t length;
    size_t capacity;
} array_header;

#define da_header(a) ((array_header *)(a)-1)
#define da_len(a) ((a) ? da_header(a)->length : 0)
#define da_cap(a) ((a) ? da_header(a)->capacity : 0)
#define da_put(a, e)                                                           \
    ((a) = arrgrowf(a, sizeof *(a), 1, 0), (a)[da_header(a)->length++] = (e))

void *arrgrowf(void *a, size_t elemsize, size_t addlen, size_t min_cap) {
    size_t len_required = da_len(a) + addlen;
    if (len_required > min_cap) {
        min_cap = len_required;
    }
    if (min_cap <= da_cap(a))
        return a;

    if (min_cap < 2 * da_cap(a)) {
        min_cap = 2 * da_cap(a);
    } else if (min_cap < 4) {
        min_cap = 4;
    }

    void *b = realloc(a ? da_header(a) : NULL,
                      min_cap * elemsize + sizeof(array_header));
    b = (array_header *)b + 1;

    if (a == NULL) {
        da_header(b)->length = 0;
    }
    da_header(b)->capacity = min_cap;
    return b;
}
