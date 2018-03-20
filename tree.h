// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef TREE_H
#define TREE_H

extern const int MAX_USERS;

// Inicialize the tree data scrucutre.
// Aborts with error code 1 if could not allocate memory.
void initTree();

// Free the tree and all related memeory that was allocated.
void freeTree();

// Add `id` as a child of `parent`. Returns 0 of failure, 1 on success.
// Aborts with error code 1 if could not allocate memory.
int treeAddNode(int id, int parent);

// Deletes the node id from the tree. Deletion takes onstant time,
// excluding preferences list freeing.
int treeDelNode(int id);

// Add preference `value` to node `id`.
int treeAddPreference(int id, int value);

// Remove preference `value` from node `id`.
int treeRemovePreference(int id, int value);

struct List *runMarathon(int root, int k);

#ifdef DEBUG

// Print the tree state to the screen.
void printTree();

#endif

#endif
