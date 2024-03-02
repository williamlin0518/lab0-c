#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create an empty queue */
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
    return;
}


/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // Check for an empty list or a list with a single element.


    return 0;  // Return the count of removed elements.
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */

int q_descend(struct list_head *head)
{
    return 0;
}
/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    return 0;
}
