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
    if (sp && bufsize) {
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

    struct list_head *fast = head->next, *slow = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    // 'slow' should now point to the middle element
    list_del(slow);
    element_t *element = list_entry(slow, element_t, list);
    free(element->value);
    free(element);

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        // Empty list or only one element, no duplicates possible.
        return false;
    }

    bool removed = false;
    struct list_head *current = head->next;
    while (current != head && current->next != head) {
        element_t *current_elem = list_entry(current, element_t, list);
        element_t *next_elem = list_entry(current->next, element_t, list);

        if (strcmp(current_elem->value, next_elem->value) == 0) {
            // Duplicate found, remove current and all duplicates.
            removed = true;
            struct list_head *dup = current;
            while (dup != head &&
                   strcmp(current_elem->value, next_elem->value) == 0) {
                struct list_head *next_dup = dup->next;
                list_del(dup);
                q_release_element(list_entry(dup, element_t, list));
                dup = next_dup;
                if (dup != head) {
                    next_elem = list_entry(dup, element_t, list);
                }
            }
            current = dup;  // Continue checking from the next distinct element.
        } else {
            current =
                current->next;  // Move to the next element if not a duplicate.
        }
    }

    return removed;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // Ensure the list is not empty or has only one element.
    if (head == NULL || head->next == head || head->next->next == head)
        return;

    struct list_head *current = head->next;
    while (current != head && current->next != head) {
        struct list_head *next = current->next;
        struct list_head *nextnext = next->next;

        // Swap current and next nodes
        next->next = current;
        current->next = nextnext;
        nextnext->prev = current;

        // Fix the previous pointers
        if (current == head->next) {  // This is the first pair, adjust the head
            head->next = next;
            next->prev = head;
        } else {  // Not the first pair
            current->prev->next = next;
            next->prev = current->prev;
        }
        current->prev = next;

        // Move to the next pair
        current = nextnext;
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
    return 0;
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
