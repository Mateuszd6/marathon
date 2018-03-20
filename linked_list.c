// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#include "linked_list.h"

int listEmpty (const struct List *list)
{
    // TODO: REMOVE THIS ASSERT!
    if (!list->head)
        assert(!list->tail);

    return (list->head == NULL);
}

// NOTE: These concats two lists, and makes BOTH src and dest poiting to the
// first element of list concatenation.
void listConcat(struct List* dest, struct List* src)
{
    // If first list is empty,copy the head and tail values from the second one
    if (listEmpty(dest))
    {
        dest->head = src->head;
        dest->tail = src->tail;
    }
    // If src is empty do nothing.
    else if (!listEmpty(src))
    {
        dest->tail->next = src->head;
        src->head->prev = dest->tail;

        dest->tail = src->tail;
    }

    src->head = dest->head;
    src->tail = dest->tail;
}

void listPushBack (struct List* list, int inserted_value)
{
    struct ListNode *new_node = malloc(sizeof(struct ListNode));

    // Could not allocate memory.
    if (!new_node)
        exit(1);

    new_node->value = inserted_value;
    new_node->next = NULL;
    new_node->prev = NULL;

    // If list is not empty just append to the back.
    if (!listEmpty(list))
    {
        new_node->prev = list->tail;
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
}

// NOTE: This assumes list is sorted in NON-INCREASING order!
void listInsertMaintainSortOrder(struct List* list, int value)
{
#ifdef DEBUG
    assert(listIsSorted(list));
#endif

    if (!list)
        assert(!"List pointer cannot be NULL!");

    struct ListNode *new_node = malloc(sizeof(struct ListNode));
    if (!new_node)
        exit(1);
    (* new_node) = (struct ListNode){ NULL, NULL, value };

    // If list has only one element just make one-element list.
    if (!list->head)
    {
        assert(!list->tail);

        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        int value_inserted = 0;
        listForeach(list,curr,
            {
                if (!value_inserted)
                    if (curr->value < value)
                    {
                        // If 'curr' is not the first element in the list
                        if (curr->prev)
                            curr->prev->next = new_node;
                        // If it is, update list head.
                        else
                            list->head = new_node;

                        new_node->prev = curr->prev;
                        new_node->next = curr;
                        curr->prev = new_node;

                        // If we insert before first element, we must update list head.
                        if (curr == list->head)
                            list->head = new_node;

                        // Make sure that all this happens only once.
                        value_inserted = 1;
                    }
            });

        if (!value_inserted)
        {
            list->tail->next = new_node;
            new_node->prev = list->tail;
            list->tail = new_node;
        }
    }
}

// TODO: Decide if doing foreach stuff is cool or not.
#if 0
void listForeach(const struct List* list, void (*func)(struct ListNode*))
{
    if (!list)
        assert(!"List pointer cannot be NULL!");

    struct ListNode *current = list->head;
    while (current != NULL)
    {
        struct ListNode *next = current->next;
        if (next == NULL)
            assert(current == list->tail);

        func(current);

        current = next;
    }
}
#endif

int listContainsElement(const struct List* list, int search_value)
{
    int element_found = 0;
#if 0
    listForeach(list,
        ({
            void callback(struct ListNode *curr)
            {
                if (curr->value == search_value)
                    element_found = 1;
            };
            callback;
        }));
#else
    listForeach(list,curr,
        {
            if (curr->value == search_value)
                element_found = 1;
        });
#endif

    return element_found;
}

void listFree(struct List* list)
{
    if (!list)
        assert(!"List pointer cannot be NULL!");
#if 0
    struct ListNode *current = list->head;
    while (current != NULL)
    {
        struct ListNode *next = current->next;
        if (next == NULL)
            assert(current == list->tail);

        free(current);

        current = next;
    }
#else
    /* listForeach(list, (void (*) (struct ListNode *))free); */
    listForeach(list,curr, { free(curr); });
#endif
    free(list);
    list = NULL;
}

// Removes the node [el] from the list.
void listRemoveNode(struct List *list, struct ListNode *el)
{
    // If this is the only element in the list:
    if (!el->prev && !el->next)
    {
        list->head = NULL;
        list->tail = NULL;
    }
    // If this is a first one:
    else if (!el->prev)
    {
        list->head = el->next;
        el->next->prev = NULL;
    }
    // If this si a last one:
    else if (!el->next)
    {
        list->tail = el->prev;
        el->prev->next = NULL;
    }
    else
    {
        el->prev->next = el->next;
        el->next->prev = el->prev;
    }

    free(el);
}

// Removes ALL ocurrences of 'value_to_remove' from the 'list'.
int listRemoveElement(struct List* list, int value_to_remove)
{
    if (!list)
        assert(!"List pointer cannot be NULL!");

    int has_removed_element = 0;
    struct ListNode *current = list->head;

    while (current != NULL)
    {
        struct ListNode *next = current->next;
        if (next == NULL)
            assert(current == list->tail);

        if (current->value == value_to_remove)
        {
            listRemoveNode(list, current);
            has_removed_element = 1;
        }

        current = next;
    }

    return has_removed_element;
}

#ifdef DEBUG

// 1 if given list is sorted in NON-INCREASING order, else 0.
int listIsSorted(const struct List* list)
{
    // Empty list is sorted.
    if (!list)
        assert(!"List pointer cannot be NULL!");

    struct ListNode *current = list->head;
    while (current != NULL)
    {
        struct ListNode *next = current->next;
        if (next && current->value < next->value)
            return 0;

        current = next;
    }

    return 1;
}

void listPrintContent(const struct List* list)
{
    listForeach(list,curr, { printf("%d ", curr->value); });
}

#endif
