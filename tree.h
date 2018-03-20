// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

extern const int MAX_USERS;

void freeTree();

void initTree();

int treeAddNode(int id, int parent);

int treeDelNode(int id);

int treeAddPreference(int id, int value);

int treeRemovePreference(int id, int value);

#ifdef DEBUG

void printTree();

#endif
