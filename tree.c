// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h> // for memeset
#include <assert.h>

#include "linked_list.h"
#include "tree.h"
#include "utils.h"

const int MAX_USERS = 65535;

struct TreeNode
{
    int id, parent;

    struct List *preferences;
    struct List *childs;

    // Position in the child list of the parent node, for O(1) node deletion.
    struct ListNode *pos_in_childlist;
};

struct TreeNode** tree_nodes;

void
allocateTreeNodesArr()
{
    tree_nodes = malloc(sizeof(struct TreeNode *) * (MAX_USERS+1));
    if (!tree_nodes)
        exit(1);

    memset(tree_nodes, 0, sizeof(struct TreeNode *) * (MAX_USERS+1));
}

// Free the given node and all its childs.
void
freeTreeNode(int node_id)
{
    struct TreeNode *tree_node = tree_nodes[node_id];
    assert(tree_node);
    listForeach(tree_node->childs,curr, { freeTreeNode(curr->value); });

    listFree(tree_node->preferences);
    listFree(tree_node->childs);
    free(tree_node);
    tree_nodes[node_id] = NULL;
}

void
initTree()
{
    allocateTreeNodesArr();

    // Add user 0.
    struct TreeNode *root = malloc(sizeof(struct TreeNode));
    struct List *childs = malloc(sizeof(struct List));
    struct List *preferences = malloc(sizeof(struct List));
    if (!root || !childs || !preferences)
        exit(1);

    (* childs) = (struct List) { NULL, NULL };
    (* preferences) = (struct List) { NULL, NULL };

#if 0
    root->id = 0;
    root->parent = 0;
    root->preferences = preferences;
    root->childs = childs;
    root->pos_in_childlist = NULL;
#else
    (* root) = (struct TreeNode) {
        0, 0,
        preferences,
        childs,
        NULL
    };
#endif

    tree_nodes[0] = root;
}

void
freeTree()
{
    freeTreeNode(0);
    free(tree_nodes);
}

int
treeAddPreference(int id, int value)
{
    if (!tree_nodes[id] || listContainsElement(tree_nodes[id]->preferences, value))
        return 0;

    listInsertMaintainSortOrder(tree_nodes[id]->preferences, value);
    return 1;
}

int
treeRemovePreference(int id, int value)
{
    if (!tree_nodes[id])
        return 0;

    return listRemoveElement(tree_nodes[id]->preferences, value);
}

// This assumes `id` and `parent` are in range [0-MAX_USERS]!
int
treeAddNode(int id, int parent)
{
    assert(0 <= id);
    assert(0 <= parent);
    assert(id <= MAX_USERS);
    assert(parent <= MAX_USERS);

    // If node to add already exits, or parent does not:
    if (tree_nodes[id] || !tree_nodes[parent])
        return 0;

    struct TreeNode *new_node = malloc(sizeof(struct TreeNode)),
        *parent_node = tree_nodes[parent];

    if (!new_node)
        exit(1);

    tree_nodes[id] = new_node;
    assert(!listRemoveElement(parent_node->childs, id));

    struct List *childs = malloc(sizeof(struct List));
    struct List *prefs = malloc(sizeof(struct List));
    if (!childs || !prefs)
        exit(1);

    (* childs) = (struct List) { NULL, NULL };
    (* prefs) = (struct List) { NULL, NULL };

    // After a push back 'parent_node->childs->tail' point to the correct node
    listPushBack(parent_node->childs, id);
    (* new_node) = (struct TreeNode) {
        id, parent,
        prefs,
        childs,
        parent_node->childs->tail
    };

    return 1;
}

int
treeDelNode(int id)
{
    assert(0 <= id);
    assert(id <= MAX_USERS);

    // It is neither possible to remove root user, nor the node that isnt there.
    if (id == 0 || !tree_nodes[id])
        return 0;

    struct TreeNode *node_to_delete = tree_nodes[id];
    assert(node_to_delete);

    struct TreeNode *parent = tree_nodes[node_to_delete->parent];
    assert(parent);

    // TODO: Remove asserts / make sure they are not present in the final build!
    listFree(node_to_delete->preferences);
    assert(node_to_delete->pos_in_childlist->value == node_to_delete->id);
    listRemoveNode(parent->childs, node_to_delete->pos_in_childlist);
    assert(!listRemoveElement(parent->childs, node_to_delete->id));

    listConcat(tree_nodes[node_to_delete->parent]->childs, node_to_delete->childs);

    free(node_to_delete->childs);
    free(node_to_delete);
    tree_nodes[id] = NULL;

    return 1;
}

static struct List *
marathonAux(struct TreeNode *curr, int k, int max_value)
{
    assert(curr);

    struct List *res = malloc(sizeof(struct List));
    (* res) = (struct List){ NULL, NULL };

    assert(listIsSorted(curr->preferences));

    // TODO: CLARIFY!!!
    // This value will be passed as max_value recuresively to `curr` childs. It
    // is either max_value, orthe first element of the `curr->preferences` list.
    int next_limit = curr->preferences->head ?
        MAX(max_value, curr->preferences->head->value) : max_value;

    listForeach(curr->childs, node,
        {
            res = listMergeSortedLists(res,
                marathonAux(tree_nodes[node->value], k, next_limit),
                k, next_limit);
        });

    int list_size = 0;
    listForeach(res, node,
        {
            ++list_size;
        });

    // If size of the result list is less than [k], add from the current node preferences lists.
    listForeach(curr->preferences, node,
        {
            if (list_size < k && node->value > max_value)
            {
                listPushBack(res, node->value);
                list_size++;
            }
            else
            {
                break;
            }
        });

    return res;
}

// TODO: Refactor the name, and make sure it can return NULL if variables are
// out of range, or just bad.
struct List *
runMarathon(int root, int k)
{
    assert(tree_nodes[root]); // TODO: This should return an error, not abort!
    return marathonAux(tree_nodes[root], k, 0);
}

#ifdef DEBUG

void
printSubtree(int curr_id)
{
    struct TreeNode *curr = tree_nodes[curr_id];
    assert(curr);
    printf("%d [ ", curr->id);
    listForeach(curr->preferences,node, { printf("%d ", node->value); });
    printf("]: ");
    listForeach(curr->childs,node, { printf("%d ", node->value); });
    printf("\n");
    listForeach(curr->childs,node, { printSubtree(node->value); });
}

void
printTree()
{
    printf("Tree state:\n");
    printSubtree(0);
    printf("\n");
}

#endif
