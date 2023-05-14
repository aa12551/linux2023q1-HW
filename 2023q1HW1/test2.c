#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#define MAX_DEPTH 512

struct item {                         
    uint16_t i;
    struct list_head list;
};

static inline int cmpint(const void *p1, const void *p2)
{
    const uint16_t *i1 = (const uint16_t *) p1;
    const uint16_t *i2 = (const uint16_t *) p2;
    return *i1 - *i2;
}

static void list_sort_nr(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head))
        return;

    struct list_head stack[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++)
        INIT_LIST_HEAD(&stack[i]);
    int top = -1;
    list_splice_init(head, &stack[++top]);

    struct list_head partition;
    INIT_LIST_HEAD(&partition);

    while (top >= 0) {
        INIT_LIST_HEAD(&partition);
        list_splice_init(&stack[top--], &partition);
        if (!list_empty(&partition) && !list_is_singular(&partition)) {
            struct list_head list_less, list_greater;
            INIT_LIST_HEAD(&list_less);
            INIT_LIST_HEAD(&list_greater);
            struct item *pivot =
                list_first_entry(&partition, struct item, list);
            list_del(&pivot->list);
            INIT_LIST_HEAD(&pivot->list);

            struct item *itm = NULL, *is = NULL;
            list_for_each_entry_safe(itm, is, &partition, list) {
                list_del(&itm->list);
                if (cmpint(&itm->i, &pivot->i) < 0)
                    list_move(&itm->list, &list_less);
                else
                    list_move(&itm->list, &list_greater);
            }

            list_move_tail(&pivot->list, &list_less);
            if (!list_empty(&list_greater))
                list_splice_tail(&list_greater, &stack[++top]);
            if (!list_empty(&list_less))
                list_splice_tail(&list_less, &stack[++top]);
        } else {
            top++;
            list_splice_tail(&partition, &stack[top]);
            while (top >= 0 && list_is_singular(&stack[top])) {
                struct item *tmp =
                    list_first_entry(&stack[top], struct item, list);
                list_del(&tmp->list);
                INIT_LIST_HEAD(&stack[top--]);
                list_add_tail(&tmp->list, head);
            }
        }
    }
}

static void list_rand(struct list_head *head, int num)
{
	for(int i = 0; i<num; i++) {
		struct item *itm = (struct item *) malloc(sizeof(struct item));
		itm->i = (rand()/1000);
		list_add_tail(&(itm->list), head);
	}
}

int main()
{
	struct list_head head;
	INIT_LIST_HEAD(&head);
	
	list_rand(&head, 5);
	
	list_sort_nr(&head);
	
	struct item *itm = NULL;
	list_for_each_entry(itm, &head, list)
		printf("%d \n",itm->i);  
	
	
	return 0;
}
