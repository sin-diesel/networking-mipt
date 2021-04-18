
#include <stddef.h>

struct list
{
    struct list *next;
    struct list *prev;
};

#define LIST_HEAD(name) struct list name = {&(name), &(name)}


#define list_for_each_entry(pos, head, member) \
    for (pos = container_of((head)->next, typeof(*pos), member);\
        &pos->member != (head);             \
        pos=container_of(pos->member.next, typeof(*pos), member));\


#define container_of(ptr, type, member) \
        ({const typeof(((type*)0)->member) *__mptr = (ptr); \
         (type*)((char*)__mptr - offsetof(type, member)); })



#define list_entry(ptr, type, member)
        container_of(ptr, type, member)


static inline void __list_add(struct list* item, struct list* prev, struct list* next)
{
    prev->next = item;
    next->prev = item;
    item->prev = prev;
    item->next = next;
}

static inline void list_init(struct list * list)
{
    list->prev = list;
    list->next = list;
}

static inline void list_add(struct list* new, struct list* head)
{
    __list_add(new, head, head->next);
}
static inline void list_add_back(struct list* new, struct list* head) //?
{
    __list_add(new, head->prev, head);
}

static inline void list_del(struct list* old)
{
    old->prev->next = old->next;
    old->next->prev = old->prev;
}
