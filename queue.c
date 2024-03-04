#include "queue.h"
#include <limits.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <string.h>
/* Create an empty queue */

struct list_head;
#ifndef _LINUX_LIST_SORT_H
#define _LINUX_LIST_SORT_H
typedef int (*list_cmp_func_t)(void *,
                               const struct list_head *,
                               const struct list_head *);

void list_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);

#endif

struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);  // Initialize the list to point to itself
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *current, *tmp;


    list_for_each_safe (current, tmp, head) {
        element_t *element = list_entry(current, element_t, list);
        list_del(current);     // Remove from list
        free(element->value);  // Free the string
        free(element);         // Free the element
    }

    free(head);  // Free the queue head
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;

    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return false;
    }

    list_add(&new_element->list, head);  // Insert at head
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;

    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element)
        return false;

    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return false;
    }

    list_add_tail(&new_element->list, head);  // Insert at tail
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *first = head->next;
    element_t *element = list_entry(first, element_t, list);

    list_del(first);  // Remove from list

    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return element;
}
/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    struct list_head *last = head->prev;  // Assuming 'head' is circular
    element_t *element = list_entry(last, element_t, list);

    // Copy string to provided buffer
    if (sp) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    // Unlink and return the element
    list_del(last);
    return element;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    struct list_head *iter;
    int count = 0;
    list_for_each (iter, head) {
        count++;
    }
    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *forward = head->next;
    struct list_head *backward = head->prev;
    while (forward != backward && forward->prev != backward) {
        forward = forward->next;
        backward = backward->prev;
    }

    list_del(forward);
    element_t *element = list_entry(forward, element_t, list);
    free(element->value);
    free(element);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return false;
    }
    element_t *current_element, *next_element;
    list_for_each_entry_safe (current_element, next_element, head, list) {
        bool is_duplicate = false;

        // Keep removing next elements as long as they are duplicates
        while (&next_element->list != head &&
               !strcmp(current_element->value, next_element->value)) {
            list_del(&next_element->list);
            free(next_element->value);
            free(next_element);
            is_duplicate = true;
            next_element =
                list_entry(current_element->list.next, element_t, list);
        }
        // If the current element was part of a duplicate sequence, remove it as
        // well
        if (is_duplicate) {
            list_del(&current_element->list);
            free(current_element->value);
            free(current_element);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (head == NULL || list_empty(head)) {
        return;
    }
    struct list_head *first = head->next;
    while (first != head && first->next != head) {
        struct list_head *second = first->next;  // The node to swap with.

        // Remove 'second' from its current position.
        list_del(first);

        // Insert 'first' after 'second '
        list_add(first, second);


        // since 'first' is now after 'second'.
        first = first->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *current = head, *temp = NULL;
    do {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;

        current = temp;  // Move to what was originally the next node
    } while (current != head);
}

/* Reverse the nodes of the list k at a time */


void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || k < 2) {
        return;
    }
    int count = 0;
    struct list_head *cut_point, *safe, *start = head;
    struct list_head temp_list;
    INIT_LIST_HEAD(&temp_list);
    list_for_each_safe (cut_point, safe, head) {
        count++;
        if (count == k) {
            list_cut_position(&temp_list, start, cut_point);
            q_reverse(&temp_list);
            list_splice_init(&temp_list, start);
            count = 0;
            start = safe->prev;
        }
    }
    return;
}


void merge(struct list_head *head,
           struct list_head *left,
           struct list_head *right,
           bool descend)
{
    while (!list_empty(left) && !list_empty(right)) {
        element_t *left_entry = list_entry(left->next, element_t, list);
        element_t *right_entry = list_entry(right->next, element_t, list);
        bool condition =
            descend ? (strcmp(left_entry->value, right_entry->value) >= 0)
                    : (strcmp(left_entry->value, right_entry->value) <= 0);
        if (condition) {
            list_move_tail(left->next, head);
        } else {
            list_move_tail(right->next, head);
        }
    }

    // Append any remaining elements
    if (!list_empty(left)) {
        list_splice_tail(left, head);
    }
    if (!list_empty(right)) {
        list_splice_tail(right, head);
    }
}

/* Sort elements of queue in ascending/descending order */

void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || head->next->next == head) {
        return;
    }

    struct list_head *slow = head->next, *fast = head->next->next, left, right;
    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    INIT_LIST_HEAD(&left);
    INIT_LIST_HEAD(&right);

    list_cut_position(&left, head, slow);
    list_splice_init(head, &right);

    q_sort(&left, descend);
    q_sort(&right, descend);

    merge(head, &left, &right, descend);
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return 0;  // No action needed if the list is empty or has only one
                   // node.
    }

    int count = 0;

    struct list_head *prev, *node = head->prev;
    element_t *last_entry = list_entry(node, element_t, list);
    char *min_value = last_entry->value;

    while (node != head) {
        element_t *entry = list_entry(node, element_t, list);
        prev = node->prev;  // Save the previous node before potentially
                            // deleting the current node.

        if (strcmp(entry->value, min_value) > 0) {
            list_del(node);
            free(entry->value);
            free(entry);

        } else {
            min_value = entry->value;
            count++;  // Increment the count of nodes that were not removed.
        }
        node = prev;
    }

    return count;  // Return the count of removed nodes.
}
/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */

int q_descend(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return 0;  // No action needed if the list is empty or has only one
                   // node.
    }

    int count = 0;
    char *max_value = "";  // Assuming "" is the smallest possible value.
    struct list_head *prev, *node = head->prev;


    while (node != head) {
        element_t *entry = list_entry(node, element_t, list);
        prev = node->prev;  // Save the previous node before potentially
                            // deleting the current node.

        if (strcmp(entry->value, max_value) < 0) {
            list_del(node);
            free(entry->value);
            free(entry);

        } else {
            max_value = entry->value;
            count++;  // Increment the count of nodes that were not removed.
        }
        node = prev;
    }

    return count;  // Return the count of removed nodes.
}
/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (list_empty(head) || list_is_singular(head)) {
        return 0;  // No action needed if the list is empty or has only one
                   // element.
    }
    queue_contex_t *first_qctx = list_first_entry(head, queue_contex_t, chain);

    // Prepare a temporary list for iterative merging.
    struct list_head new_head;
    INIT_LIST_HEAD(&new_head);
    struct list_head merged;
    INIT_LIST_HEAD(&merged);

    queue_contex_t *entry, *safe;

    // Start with merging into the first queue.
    list_splice_init(first_qctx->q, &merged);

    // Iterate through the remaining queue contexts for merging.
    list_for_each_entry_safe (entry, safe, head, chain) {
        if (entry == first_qctx)
            continue;  // Skip the first queue context as it's already included.

        // Use the merge function to combine the current queue with the merged
        // results.
        struct list_head temp;
        INIT_LIST_HEAD(&temp);
        list_splice_init(entry->q, &temp);  // Prepare the current queue.
        merge(&new_head, &merged, &temp, descend);
        INIT_LIST_HEAD(&merged);
        list_splice_init(&new_head, &merged);
    }
    list_splice(&merged, first_qctx->q);

    return q_size(first_qctx->q);
}
