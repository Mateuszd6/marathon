// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Singly linked list, only pointer to the next element is stored, there is no
// way to go backwards.
struct ListNode
{
    struct ListNode *next, *prev;
    int value;
};

struct List
{
    // If head is NULL, the list is empty.
    struct ListNode *head;

    // Reference to tail is needed to be able to concat lists in O(1) time.
    // If head is NULL, this must be also NULL!
    struct ListNode *tail;
};

int listEmpty (const struct List *list);

void listConcat(struct List* dest, struct List* src);

int listContainsElement(const struct List* list, int search_value);

void listPushBack (struct List* list, int inserted_value);

void listInsertMaintainSortOrder(struct List* list, int value);

#if 0
// TODO: Decide if doing foreach stuff is cool or not.
void listForeach(const struct List* list, void (*func)(struct ListNode*));
#else
#define listForeach(list,element,func_body)             \
    {                                                   \
        if (!(list))                                    \
            assert(!"List pointer cannot be NULL!");    \
        struct ListNode *(element) = (list)->head;      \
        while ((element) != NULL)                       \
        {                                               \
            struct ListNode *next = (element)->next;    \
            if (next == NULL)                           \
                assert((element) == (list)->tail);      \
                                                        \
            (func_body);                                \
                                                        \
            (element) = next;                           \
        }                                               \
    }

#endif

void listFree(struct List* list);

void listRemoveNode(struct List *list, struct ListNode *el);

int listRemoveElement(struct List* list, int value_to_remove);

struct List *listMergeSortedLists(struct List *self, struct List *other,
    int greater_than, int max_elements);

#ifdef DEBUG

int listIsSorted(const struct List* list);

void listPrintContent(const struct List* list);

#endif

#endif
