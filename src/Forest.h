#ifndef FOREST_H
#define FOREST_H

/* Define structure for Nodes, ie: leaf */
typedef struct Node Node;
struct Node{
  int n_subnode, id_row, id_col;
  float value;
  char sign;
  Node **next;
};
 
 /* One tree for the planet */
typedef struct Tree Tree;
struct Tree{
  Node *root;
};

/* Initialize tree structure and make all memory allocation */
Tree * initialize_tree();

/* Initialize tree structure with smaller depth than theoric one and 
 * make all allocation
 */
Tree * initialize_tree_corrected_depth();

/*
 * Add information about one node to the tree
 * ie : after the player or computer plays, we need to update the tree
 * 
 */
int add_to_tree(Tree *, int, int, int, char);

/*
 * Display the tree brutally
 */
void show_tree(Node *);

#endif
