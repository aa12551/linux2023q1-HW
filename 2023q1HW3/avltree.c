#include "avltree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

long nsec = 0;

struct avl_prio_queue {
    struct avl_root root;
    struct avl_node *min_node;
};

struct avlitem {
	long i;
	struct avl_node * avl;
};

struct node_t {
	long value;
	struct node_t *next;
};
static inline struct avlitem *avl_prio_queue_create_item(long i)
{
	struct avlitem *node = malloc(sizeof(struct avlitem));
	node->avl = malloc(sizeof(struct avl_node));
	node->i = i;
	return node;
}

static inline void avl_prio_queue_init(struct avl_prio_queue *queue)
{
    INIT_AVL_ROOT(&queue->root);
    queue->min_node = NULL;
}

enum { _CMP_LESS = -1, _CMP_EQUAL = 0, _CMP_GREATER = 1 };

/* Integer comparison */
static inline int cmpint(void *arg0, void *arg1)
{
    int *a = (int *) arg0, *b = (int *) arg1;
    return (*a < *b) ? _CMP_LESS : (*a > *b) ? _CMP_GREATER : _CMP_EQUAL;
}

static inline void avl_prio_queue_insert_unbalanced(
    struct avl_prio_queue *queue,
    struct avlitem *new_entry)
{
    struct avl_node *parent = NULL;
    struct avl_node **cur_nodep = &queue->root.node;
    struct avlitem *cur_entry;
    int isminimal = 1;
    
    while (*cur_nodep) {
        cur_entry = avl_entry(*cur_nodep, struct avlitem, avl);

        parent = *cur_nodep;
        if (cmpint(&new_entry->i, &cur_entry->i) <= 0) {
            cur_nodep = &((*cur_nodep)->left);
        } else {
            cur_nodep = &((*cur_nodep)->right);
            isminimal = 0;
        }
    }

    if (isminimal)
        queue->min_node = &new_entry->avl;

    avl_link_node(&new_entry->avl, parent, cur_nodep);
}

static inline struct avlitem *avl_prio_queue_pop_unbalanced(
    struct avl_prio_queue *queue)
{
    struct avlitem *item;
    bool removed_right;

    if (!queue->min_node)
        return NULL;

    item = avl_entry(queue->min_node, struct avlitem, avl);
    queue->min_node = avl_next(queue->min_node);

    avl_erase_node(&item->avl, &queue->root, &removed_right);

    return item;
}

static inline void avl_prio_queue_insert_balanced(
    struct avl_prio_queue *queue,
    struct avlitem *new_entry)
{
    struct avl_node *parent = NULL;
    struct avl_node **cur_nodep = &queue->root.node;
    struct avlitem *cur_entry;
    int isminimal = 1;
    while (*cur_nodep) {
    	struct timespec tt1, tt2;
    	clock_gettime(CLOCK_REALTIME, &tt1);
        cur_entry = avl_entry(*cur_nodep, struct avlitem, avl);
	clock_gettime(CLOCK_REALTIME, &tt2);
    	printf("t = %ld\n",tt2.tv_nsec - tt1.tv_nsec);

        parent = *cur_nodep;
        if (cmpint(&new_entry->i, &cur_entry->i) <= 0) {
            cur_nodep = &((*cur_nodep)->left);
        } else {
            cur_nodep = &((*cur_nodep)->right);
            isminimal = 0;
        }
    }

    if (isminimal)
        queue->min_node = &new_entry->avl;

    avl_insert(&new_entry->avl, parent, cur_nodep, &queue->root);
}

static inline struct avlitem *avl_prio_queue_pop_balanced(
    struct avl_prio_queue *queue)
{
    struct avlitem *item;

    if (!queue->min_node)
        return NULL;

    item = avl_entry(queue->min_node, struct avlitem, avl);
    queue->min_node = avl_next(queue->min_node);

    avl_erase(&item->avl, &queue->root);

    return item;
}

/* shuffle array, only work if n < RAND_MAX */
static void shuffle(int *array, size_t n)
{
    if (n <= 1)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

/* Verify if list is order */
static bool arr_is_ordered(int *arr, int n)
{
    for (int i = 1; i < n; i++) {
        if (arr[i] <= arr[i-1]) { 
            return false;
        }
    }
    return true;
}

int main() {
	struct avl_prio_queue *q = malloc(sizeof(struct avl_prio_queue));
	avl_prio_queue_init(q);
	
	int count = 100;
	int *test_arr = malloc(sizeof(int) * count);
	for (int i = 0; i < count; ++i)
        test_arr[i] = i;
	shuffle(test_arr, count);
		
	for(int i = 0; i < count; i++) {
		struct avlitem *node = avl_prio_queue_create_item(test_arr[i]);
		avl_prio_queue_insert_balanced(q, node);
	}

	struct avl_node *iter = q->min_node;
	for(int i = 0; i < count; i++) {
		struct avlitem *cur_entry = avl_entry(iter, struct avlitem, avl);
		test_arr[i] = cur_entry->i;
		iter = avl_next(iter);	
	}




	printf("comsume %ld nanoseconds\n", nsec);
	assert(arr_is_ordered(test_arr, count));
	
	return 0;
}
