// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct ListNode {
  struct ListNode *next, *prev;
  int value;
};

struct List {
  // If head is NULL, the list is empty.
  struct ListNode *head;

  // If head is NULL, this must be also NULL!
  struct ListNode *tail;
};

// 1 if [list] is empty, else 0.
int listEmpty(const struct List *list);

// Print [list] content to the screen. Note: No endline char is printed.
void listPrintContent(const struct List *list);

// Free whole [list] (and its conents, of course).
void listFree(struct List *list);

// Push the given value to the end of a list. Allocates a new node and uses
// [listPushBackNode] to add it to the list. Aborts with error code 1 if
// could not allocate memeory.
void listPushBack(struct List *list, int inserted_value);

// "Copy" the content of the [src] list to the [dest]. [dest] list becomes
// empty, and all its elements are moved to [dest]. No additional memory is
// allocated.
void listConcat(struct List *dest, struct List *src);

// Insert maintaining sort order of the list. This won't insert a value if one
// is already there. This assumes list is sorted in NON-INCREASING order!
// Aborts with error code 1 if could not allocate memory.
// Returns 0 if value wasn't inserted, else 1.
int listInsertMaintainSortOrder(struct List *list, int value);

// Removes the node [el] from the list. This assmues that [el] is part of
// [list]. If it is not true, behaviour is undefined!
void listRemoveNode(struct List *list, struct ListNode *el);

// Removes ALL ocurrences of [value_to_remove] from the [list].
int listRemoveElement(struct List *list, int value_to_remove);

// Merge two sorted lists, but keep only [max_elements] or less elements and
// store values only grater than [greater_than]. Both [self], and [other] lists
// are deleted and cleared, nodes that are not in the result list are freed. No
// additional memory for nodes in the result list is allocated.
struct List *listMergeSortedLists(struct List *self, struct List *other,
                                  int greater_than, int max_elements);

// Macro used to execute some code for each node in a list.
#define listForeach(list, element, func_body)                                  \
  {                                                                            \
    if (!(list))                                                               \
      assert(!"List pointer cannot be NULL!");                                 \
    struct ListNode *(element) = (list)->head;                                 \
    while ((element) != NULL) {                                                \
      struct ListNode *next = (element)->next;                                 \
      if (next == NULL)                                                        \
        assert((element) == (list)->tail);                                     \
                                                                               \
      (func_body);                                                             \
                                                                               \
      (element) = next;                                                        \
    }                                                                          \
  }

#ifdef DEBUG

// 1 if given list is sorted in NON-INCREASING order, else 0.
// Used only for debugging, not included in a release build.
int listIsSorted(const struct List *list);

#endif

#endif
