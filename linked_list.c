// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef DEBUG
#define NDEBUG
#endif

#include <stdlib.h>
#include <malloc.h>
#include <assert.h>

#include "linked_list.h"
#include "utils.h"

// Insert node to the back of the list. Assumes the node is not in any list!
static void
listPushBackNode(struct List* list, struct ListNode *node_to_insert)
{
    node_to_insert->next = NULL;
    node_to_insert->prev = NULL;

    // If list is not empty just append to the back.
    if (!listEmpty(list))
    {
        node_to_insert->prev = list->tail;
        list->tail->next = node_to_insert;

        list->tail = node_to_insert;
    }
    // Otherwise just add it as first element and set both head and tail to
    // point to it.
    else
    {
        list->head = node_to_insert;
        list->tail = node_to_insert;
    }
}

int listEmpty (const struct List *list)
{
    if (!list->head)
        assert(!list->tail);

    return (list->head == NULL);
}

void listPrintContent(const struct List* list)
{
    listForeach(list,curr,
        {
            printf("%d", curr->value);

            // Dont print space after last number.
            if (curr->next != NULL)
                printf(" ");
        });
}

void listFree(struct List* list)
{
    if (!list)
        assert(!"List pointer cannot be NULL!");

    listForeach(list,curr, { free(curr); });

    // TODO: Would be nice to renameto listCrear once decided not to do it here.
    free(list);
    list = NULL;
}

void listPushBack(struct List* list, int inserted_value)
{
    struct ListNode *new_node = malloc(sizeof(struct ListNode));

    // Could not allocate memory.
    if (!new_node)
        exit(1);

    new_node->value = inserted_value;
    listPushBackNode(list, new_node);
}

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

    // [src] is now empty, all its elements were moved to [dest].
    src->head = NULL;
    src->tail = NULL;
}

int listInsertMaintainSortOrder(struct List* list, int value)
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

    int value_inserted = 0,
        value_already_in_list = 0;

    listForeach(list,curr,
        {
            if (!value_inserted && !value_already_in_list)
            {
                if (curr->value == value)
                {
                    value_already_in_list = 1;
                }
                else if (curr->value < value)
                {
                    // If 'curr' is not the first element in the list,
                    // we do normally.
                    if (curr->prev)
                        curr->prev->next = new_node;
                    // If it is, update list head.
                    else
                        list->head = new_node;

                    // If inserting as first element, curr->prev is NULL,
                    // and everything is fine.
                    new_node->prev = curr->prev;

                    new_node->next = curr;
                    curr->prev = new_node;

                    // Make sure that all this happens only once.
                    value_inserted = 1;
                }
            }
        });

    // If value was not inserted its the least element in the list,
    // (`list` might be also empty), so we append it to the back.
    if (!value_inserted && !value_already_in_list)
    {
        listPushBackNode(list, new_node);
        value_inserted = 1;
    }
    // TODO: Weird, change the behaviour!!!
    else if (!value_inserted && value_already_in_list)
    {
        free(new_node);
    }

    return value_inserted;
}


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
    // If this is a last one:
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

struct List *
listMergeSortedLists(struct List *self, struct List *other,
                     int greater_than, int max_elements)
{
#ifdef DEBUG
    assert(listIsSorted(self));
    assert(listIsSorted(other));
#endif

    struct List *res = malloc(sizeof(struct List));
    (* res) = (struct List){ NULL, NULL };

    struct ListNode *self_curr = self->head,
                    *other_curr = other->head;

    int inserted_elements = 0;

    while ((self_curr || other_curr) && inserted_elements < max_elements)
    {
        if (!self_curr || (other_curr && self_curr->value < other_curr->value))
        {
            SWAP(self_curr, other_curr);
            continue;
        }

        if (other_curr && self_curr->value == other_curr->value)
        {
            struct ListNode *next = other_curr->next;
            free(other_curr);
            other_curr = next;
            continue;
        }

        assert(!other_curr || self_curr->value > other_curr->value);

        if (self_curr->value <= greater_than)
            break;

        struct ListNode *next = self_curr->next;
        listPushBackNode(res, self_curr);
        self_curr = next;

        ++inserted_elements;
    }

    for (int i = 0; i < 2; ++i)
    {
        while (self_curr)
        {
            struct ListNode *next = self_curr->next;
            free(self_curr);
            self_curr = next;
        }
        SWAP(self_curr, other_curr);
    }

    // TODO: REFACTOR THE list API?
    free(self);
    free(other);
    self = NULL;
    other = NULL;

    return res;
}

#ifdef DEBUG

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

#endif
