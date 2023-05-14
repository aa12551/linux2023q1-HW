#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

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

static void list_sort(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head))
        return;

    struct list_head list_less, list_greater;
    INIT_LIST_HEAD(&list_less);
    INIT_LIST_HEAD(&list_greater);

    struct item *pivot = list_first_entry(head, struct item, list);
    list_del(&pivot->list);

    struct item *itm = NULL, *is = NULL;
    list_for_each_entry_safe(itm, is, head, list) {
        if (cmpint(&itm->i, &pivot->i) < 0)
            list_move_tail(&itm->list, &list_less);
        else
            list_move_tail(&itm->list, &list_greater);
    }

    list_sort(&list_less);
    list_sort(&list_greater);

    list_add(&pivot->list, head);
    list_splice(&list_less, head);
    list_splice_tail(&list_greater, head);
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
	
	list_sort(&head);
	
	struct item *itm = NULL;
	list_for_each_entry(itm, &head, list)
		printf("%d \n",itm->i);  
	
	
	return 0;
}
