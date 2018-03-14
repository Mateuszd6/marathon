// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#include <malloc.h>
#include <assert.h>

// Singly linked list, only pointer to the next element is stored, there is no
// way to go backwards.
struct ListNode
{
    // Last element points to NULL.
    struct ListNode *next;

    int value;
};

struct List
{
    // If head is NULL, the list is empty.
    struct ListNode *head;

    // Reference to tail is needed to be able to append elements to the end of
    // the list in O(1) time.
    struct ListNode *tail;
};

int listEmpty (const struct List *list)
{
    // TODO: REMOVE THIS ASSERT!
    if (!list->head)
        assert(!list->tail);

    return (list->head == NULL);
}

void listClear(const struct List *list)
{
    if (listEmpty(list))
        return; // The list is already empty!

    struct ListNode *current = list->head;

    while (current)
    {
        struct ListNode *next = current->next;
        free(current);
        current = next;
    }
}

void listConcat(struct List* dest, const struct List* src)
{
    if (listEmpty(dest))
    {
        // If first list is empty, copy the head and tail
        // values from the second one.
        dest->head = src->head;
        dest->tail = src->tail;
    }
    else
    {
        dest->tail->next = src->head;
    }
}

int listPushBack (struct List* list, int inserted_value)
{
    struct ListNode *new_node = malloc(sizeof(struct ListNode));

    // Could not allocate memory.
    if (!new_node)
        return -1;

    new_node->value = inserted_value;
    new_node->next = NULL;

    // If list is not empty just append to the back.
    if (!listEmpty(list))
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    // Otherwise just add it as first element and set both head and tail to
    // point to it.
    else
    {
        list->head = new_node;
        list->tail = new_node;
    }

    return 0;
}
