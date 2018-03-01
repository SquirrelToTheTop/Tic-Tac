#include <stdlib.h>

#include "Forest.h"
#include "Params.h"

/* Initialize the tree since there is NCELL x NCELL
 * the first node of the tree will have (NCELL x NCELL)-1
 * the second ((NCELL*NCELL)-1)-1 etc ... we'll allocate the
 * memory in consequence and hope for the best !
 *
 * headache with this
 *
 */
Tree * initialize_tree(){

  int i, j, k, n_lvl, current_n_leaf, next_n_leaf;
  int leaf_by_lvl[NCELL*NCELL];

  Tree *tree;
  Node *current, *first;

  /* number of level of the tree */
  n_lvl = NCELL*NCELL;

  /* allocate the tree =) */
  tree = (Tree *) malloc(sizeof(Tree *));
  if( !tree )
    printf("Allocation failed !");
  else
    printf("Allocation of Tree success - mem adresse %p\n", tree);

  /* compute leaf by level */
  leaf_by_lvl[0] = 1;
  for(i=1; i<n_lvl; i++)
    leaf_by_lvl[i] = n_lvl-i;

#ifdef DEBUG
  for(i=0; i<n_lvl; i++)
    printf("---> lvl %d -> n_leaf = %d\n", i, leaf_by_lvl[i]);
  printf("\n");
#endif

  /* allocate the first node of the tree, ie : root */
  current_n_leaf = NCELL*NCELL-1;
  first = (Node *) malloc(sizeof(Node *));
  //first->n_subnode = current_n_leaf;

  //first->next = (Node **) malloc(first->n_subnode * sizeof(Node *));

  first->previous = NULL;
  tree->root = first;

#ifdef DEBUG
  printf("|| -> Root at %p \n",tree->root);
#endif

  /* start allocation of the whole tree */
  current = first;
  n_lvl = 2;//NCELL*NCELL;
  next_n_leaf = current_n_leaf -1;
  for(i=0; i<n_lvl-1; i++){
    printf("Level i=%d, n_leaf = %d\n",i, current_n_leaf);
    for(j=0; j<leaf_by_lvl[i]; j++){
      current->n_subnode = leaf_by_lvl[i+1];
      current->next = (Node **) malloc((leaf_by_lvl[i+1])*sizeof(Node **));
      for(k=0; k<leaf_by_lvl[i+1]; k++){
	Node *ptr_tmp=NULL;

	ptr_tmp = (Node *) malloc(sizeof(Node *));
	if( !ptr_tmp )
	  exit(EXIT_FAILURE);

	ptr_tmp->value = 0;
	ptr_tmp->n_subnode = 0;
	ptr_tmp->next = NULL;
	ptr_tmp->previous = current;
	current->next[k] =&ptr_tmp;

#ifdef DEBUG
	printf("|||| -> Node %p as leaf %p \n",current, current->next[k]);
#endif
	free(ptr_tmp);
      }
      current = current->next[j];
    }
  }

  /* the next pointor of all final node should be set to nul */
  /* TODO but how ??? */

  return tree;
}

/*
 * Display the tree - used for debugging purpose !
 *
 */
void show_tree(Tree *bonzai){
  int i, j, count_lvl, count_leaf=0;
  Node *current;

  count_lvl = 0;
  current = bonzai->root;
  count_leaf = current->n_subnode;

  printf("\n");
  while(current->next[0] != NULL){
    printf("-----> leaf : %p having % sub-leaf \n", current, current->n_subnode);

    if( count_leaf == current->n_subnode){
      count_leaf = 0;
      current = current->next[count_leaf];
    }else
      count_leaf ++;

  }
}
