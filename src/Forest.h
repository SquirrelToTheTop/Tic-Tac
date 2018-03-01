#ifndef FOREST_H
#define FOREST_H

/* Define structure for Nodes, ie: leaf */
typedef struct Node Node;
struct Node{
  int n_subnode;
  float value;
  Node **next;
  Node *previous;
};
 
 /* One tree for the planet */
typedef struct Tree Tree;
struct Tree{
  Node *root;
};

Tree * initialize_tree();
void show_tree(Tree *);

#endif
