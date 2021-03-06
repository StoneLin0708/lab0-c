#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    /* TODO: How about freeing the list elements and the strings? */
    /* Free queue structure */
    while (q->head) {
        free(q->head->value);
        list_ele_t *next = q->head->next;
        free(q->head);
        q->head = next;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* TODO: What should you do if the q is NULL? */
    if (newh) {
        const size_t l = strlen(s) + 1;
        /* Don't forget to allocate space for the string and copy it */
        /* What if either call to malloc returns NULL? */
        newh->value = malloc(l);
        if (newh->value) {
            strncpy(newh->value, s, l);  // copy string
            newh->next = q->head;
            q->head = newh;
            if (!q->tail)  // first element
                q->tail = newh;
            ++(q->size);
            return true;
        }
        free(newh);
    }
    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    /* TODO: What should you do if the q is NULL? */
    if (newh) {
        const size_t l = strlen(s) + 1;
        /* Don't forget to allocate space for the string and copy it */
        /* What if either call to malloc returns NULL? */
        newh->value = malloc(l);
        if (newh->value) {
            strncpy(newh->value, s, l);  // copy string
            newh->next = NULL;
            if (q->tail)
                q->tail->next = newh;
            q->tail = newh;
            if (!q->head)  // first element
                q->head = newh;
            ++(q->size);
            return true;
        }
        free(newh);
    }
    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q)
        return false;
    if (!q->head)
        return false;
    list_ele_t *node = q->head;
    q->head = node->next;
    if (q->tail == node)
        q->tail = NULL;
    size_t len = strlen(node->value);
    if (sp) {
        strncpy(sp, node->value, bufsize - 1);
        if (len > (bufsize - 1)) {
            sp[bufsize - 1] = 0;
        }
    }
    free(node->value);
    free(node);
    --(q->size);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return !q ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q)
        return;
    list_ele_t *cur = NULL;
    q->tail = q->head;
    while (q->head) {
        list_ele_t *next = q->head->next;
        q->head->next = cur;
        cur = q->head;
        q->head = next;
    }
    q->head = cur;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *l, list_ele_t *r)
{
    list_ele_t *h;
    if (strcmp(l->value, r->value) < 0) {
        h = l;
        l = l->next;
    } else {
        h = r;
        r = r->next;
    }
    list_ele_t *c = h;
    while (l && r) {
        if (strcmp(l->value, r->value) < 0) {
            c->next = l;
            l = l->next;
        } else {
            c->next = r;
            r = r->next;
        }
        c = c->next;
    }
    c->next = l ? l : r;
    return h;
}

int comp(const void *a, const void *b)
{
    return strcmp((*(list_ele_t **) a)->value, (*(list_ele_t **) b)->value);
}

list_ele_t *quick_sort_sub(list_ele_t **l)
{
    list_ele_t *arr[512];          // create array of elements
    list_ele_t **end = &arr[512];  // set maximum
    list_ele_t **begin;
    // put list of elements into array, also find last element
    for (begin = arr; (begin != end) && *l; *l = (*l)->next, ++begin)
        *begin = *l;
    end = begin;
    qsort(arr, end - arr, sizeof(list_ele_t *), comp);  // sort
    for (begin = arr; (begin != end - 1);
         ++begin)  // put array of elements back to list
        (*begin)->next = *(begin + 1);
    (*begin)->next = NULL;  // cut last
    return *arr;
}

void q_sort(queue_t *q)
{
    const size_t s = q_size(q);
    if (!s)
        return;
    list_ele_t *head = q->head;
    list_ele_t *sorted = NULL;
    while (head) {  // terminate when no element left
        // quick sort sub-list and modify head to next element after sorted list
        list_ele_t *newh = quick_sort_sub(&head);
        // merge two list if exist
        sorted = sorted ? merge(sorted, newh) : newh;
    }
    q->head = sorted;
}
