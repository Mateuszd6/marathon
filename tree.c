// Mateusz Dudziński
// IPP, 2018L Task: "Maraton filmowy".

#ifndef DEBUG
#define NDEBUG
#endif

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <stdint.h>

#include "linked_list.h"
#include "tree.h"
#include "utils.h"

struct TreeNode {
  int id, parent;

  struct List *preferences;
  struct List *childs;

  // Position in the child list of the parent node, for O(1) node deletion.
  struct ListNode *pos_in_childlist;
};

// Free the given node and all its childs.
static void freeTreeNode(struct Tree tree, int node_id) {
  struct TreeNode *tree_node = tree.nodes[node_id];
  assert(tree_node);
  listForeach(tree_node->childs, curr, { freeTreeNode(tree, curr->value); });

  listFree(tree_node->preferences);
  listFree(tree_node->childs);
  free(tree_node);
  tree.nodes[node_id] = NULL;
}

struct Tree initTree(int32_t number_of_nodes) {
  struct TreeNode **tree_nodes =
      malloc(sizeof(struct TreeNode *) * number_of_nodes);
  if (!tree_nodes)
    exit(1);

  // Clear the allocatted array.
  memset(tree_nodes, 0, sizeof(struct TreeNode *) * number_of_nodes);

  // Add user 0.
  struct TreeNode *root = malloc(sizeof(struct TreeNode));
  struct List *childs = malloc(sizeof(struct List));
  struct List *preferences = malloc(sizeof(struct List));
  if (!root || !childs || !preferences)
    exit(1);

  (*childs) = (struct List){NULL, NULL};
  (*preferences) = (struct List){NULL, NULL};
  (*root) = (struct TreeNode){0, 0, preferences, childs, NULL};

  tree_nodes[0] = root;

  struct Tree res = {tree_nodes, number_of_nodes};
  return res;
}

void freeTree(struct Tree tree) {
  freeTreeNode(tree, 0);
  free(tree.nodes);

  tree.nodes = NULL;
}

int treeAddNode(struct Tree tree, int id, int parent) {
  if (!inRange(0, tree.size, id) || !inRange(0, tree.size, parent))
    return 0;

  // If node to add already exits, or parent does not:
  if (tree.nodes[id] || !tree.nodes[parent])
    return 0;

  struct TreeNode *new_node = malloc(sizeof(struct TreeNode)),
                  *parent_node = tree.nodes[parent];

  if (!new_node)
    exit(1);

  tree.nodes[id] = new_node;

#ifdef DEBUG
  assert(!listRemoveElement(parent_node->childs, id));
#endif

  struct List *childs = malloc(sizeof(struct List));
  struct List *prefs = malloc(sizeof(struct List));
  if (!childs || !prefs)
    exit(1);

  (*childs) = (struct List){NULL, NULL};
  (*prefs) = (struct List){NULL, NULL};

  // After a push back [parent_node->childs->tail] point to the correct node.
  listPushBack(parent_node->childs, id);
  (*new_node) =
      (struct TreeNode){id, parent, prefs, childs, parent_node->childs->tail};

  return 1;
}

int treeDelNode(struct Tree tree, int id) {
  if (!inRange(0, tree.size, id))
    return 0;

  // It is neither possible to remove root user, nor the node that isnt there.
  if (id == 0 || !tree.nodes[id])
    return 0;

  struct TreeNode *node_to_delete = tree.nodes[id];
  struct TreeNode *parent = tree.nodes[node_to_delete->parent];
  assert(parent);

  listForeach(node_to_delete->childs, node,
              { tree.nodes[node->value]->parent = parent->id; });

  // Free the preferences list.
  listFree(node_to_delete->preferences);

  assert(node_to_delete->pos_in_childlist->value == node_to_delete->id);

  // Now we remove the node from the list, so is is not there anymore.
  listRemoveNode(parent->childs, node_to_delete->pos_in_childlist);
#ifdef DEBUG
  assert(!listRemoveElement(parent->childs, node_to_delete->id));
#endif

  // The childlist of the deleted node is appended to its parent.
  listConcat(tree.nodes[node_to_delete->parent]->childs,
             node_to_delete->childs);

  free(node_to_delete->childs);
  free(node_to_delete);
  tree.nodes[id] = NULL;

  return 1;
}

int treeAddPreference(struct Tree tree, int id, int32_t value) {
  if (!tree.nodes[id] || value < 0)
    return 0;

  return listInsertMaintainSortOrder(tree.nodes[id]->preferences, value);
}

int treeRemovePreference(struct Tree tree, int id, int32_t value) {
  if (!tree.nodes[id] || value < 0)
    return 0;

  return listRemoveElement(tree.nodes[id]->preferences, value);
}

static struct List *marathonAux(struct Tree tree, struct TreeNode *curr,
                                int32_t k, int32_t max_value) {
  assert(curr);

  struct List *res = malloc(sizeof(struct List));
  (*res) = (struct List){NULL, NULL};

#ifdef DEBUG
  assert(listIsSorted(curr->preferences));
#endif

  // This value will be passed as max_value recuresively to [curr] childs. It
  // is max of either [max_value], or first element of the [curr->preferences]
  // list (if one exists).
  int next_limit = curr->preferences->head
                       ? MAX(max_value, curr->preferences->head->value)
                       : max_value;

  listForeach(curr->childs, node, {
    struct List *partial_res;
    partial_res = marathonAux(tree, tree.nodes[node->value], k, next_limit);
    res = listMergeSortedLists(res, partial_res, next_limit, k);
  });

  int list_size = 0;
  listForeach(res, node, { ++list_size; });

  // If size of the result list is less than [k], add from the current node
  // preferences lists.
  listForeach(curr->preferences, node, {
    if (list_size < k && node->value > max_value) {
      listPushBack(res, node->value);
      list_size++;
    } else {
      break;
    }
  });

  return res;
}

struct List *runMarathon(struct Tree tree, int root, int32_t k) {
  if (!tree.nodes[root] || k < 0)
    return NULL;

  return marathonAux(tree, tree.nodes[root], k, -1);
}

#ifdef DEBUG

static void printSubtree(struct Tree tree, int curr_id) {
  struct TreeNode *curr = tree.nodes[curr_id];
  assert(curr);
  printf("%d [ ", curr->id);
  listForeach(curr->preferences, node, { printf("%d ", node->value); });
  printf("]: ");
  listForeach(curr->childs, node, { printf("%d ", node->value); });
  printf("\n");
  listForeach(curr->childs, node, { printSubtree(tree, node->value); });
}

void printTree(struct Tree tree) {
  printf("Tree state:\n");
  printSubtree(tree, 0);
  printf("\n");
}

#endif
