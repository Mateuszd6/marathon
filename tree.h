// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef TREE_H
#define TREE_H

// We represent tree as an array of nodes, coz it is the only way we can access
// any vertex in constant time.
struct Tree {
  struct TreeNode **nodes;
  int size;
};

// Inicialize the tree data scrucutre.
// Aborts with error code 1 if could not allocate memory.
struct Tree initTree(int size);

// Free the tree and all related memeory that was allocated.
void freeTree(struct Tree tree);

// Add [id] as a child of [parent]. Returns 0 of failure, 1 on success.
// Aborts with error code 1 if could not allocate memory.
int treeAddNode(struct Tree tree, int id, int parent);

// Deletes the node id from the tree. Deletion takes onstant time,
// excluding preferences list freeing.
int treeDelNode(struct Tree tree, int id);

// Add preference [value] to node [id].
int treeAddPreference(struct Tree tree, int id, int value);

// Remove preference [value] from node [id].
int treeRemovePreference(struct Tree tree, int id, int value);

struct List *runMarathon(struct Tree tree, int root, int k);

#ifdef DEBUG

// Print the tree state to the screen.
void printTree(struct Tree tree);

#endif

#endif
