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

  /* local variables */
  int i, j, k, n_lvl;
  int leaf_by_lvl[NCELL*NCELL];

  Tree *tree;
  Node *current, *first;

  /* number of level of the tree */
  n_lvl = NCELL*NCELL;

  /* test allocation the tree =) */
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
  first = (Node *) malloc(sizeof(Node *));
  first->n_subnode = leaf_by_lvl[0];
  first->previous = NULL;
  tree->root = first;

#ifdef DEBUG
  printf("|| -> Root at %p \n",tree->root);
#endif

  /* start allocation of the whole tree */
  current = first;

#ifdef DEBUG
  n_lvl = 4;//NCELL*NCELL;
#else
  n_lvl = NCELL*NCELL;
#endif
  
  int n_leaf_2_current;
  
  /* for all lvl */
  for(i=0; i<n_lvl-1; i++){
    
#ifdef DEBUG
  printf("---> working on lvl %d\n", i);
#endif
	
      /* for all leaf in the lvl */
      int tmp_n_subnode = current->n_subnode;
      for(j=0; j<tmp_n_subnode; j++){
	
	
	/* allocate is array of leaf with number of leaf */
	current->next = (Node **) malloc(leaf_by_lvl[i+1] * sizeof(Node **));
	
	/* set number of leaf of the current leaf, ie leaf j */
	current->n_subnode = leaf_by_lvl[i+1];

#ifdef DEBUG
  printf("\t---> Node %p has %d leafs \n", current, current->n_subnode);
#endif

	/* allocate all next leaf */
	for(k=0; k<current->n_subnode; k++){
	  current->next[k] = (Node *) malloc(sizeof(Node *));
	  (current->next[k])->next = NULL;
	  (current->next[k])->previous = current;

#ifdef DEBUG
  printf("\t\t---> leaf at %p \n", current->next[k]);
#endif
	}
	
	if( current->previous ) 
	  current = (current->previous)->next[j];
	else
	  current = current->next[j];
	
      }
     
     // WARNING this makes infinite loop : current = current->next[0];
     
      /* after finishing all leaf of a level do to sub level */
     
  }

  /* the next pointor of all final node should be set to nul */
  /* TODO but how ??? */

  return tree;
}

/* Initialize the tree since there is NCELL x NCELL
 * the first node of the tree will have (NCELL x NCELL)-1
 * the second ((NCELL*NCELL)-1)-1 etc ... we'll allocate the
 * memory in consequence and hope for the best !
 *
 * headache with this
 *
 */
Tree * initialize_tree_test(){

  /* local variables */
  int i, j, k, n_lvl;
  int leaf_by_lvl[NCELL*NCELL];

  Tree *tree;
  Node *current, *first;

  /* number of level of the tree */
  n_lvl = NCELL*NCELL;

  /* test allocation the tree =) */
  tree = (Tree *) malloc(sizeof(Tree *));
  if( !tree ){
    printf("Allocation failed !");
    exit(EXIT_FAILURE);
  }else
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
  first = (Node *) malloc(sizeof(Node *));
  first->previous = NULL;
  
  /* set first node as root of tree */
  tree->root = first;

#ifdef DEBUG
  printf("|| -> Root at %p \n", tree->root);
#endif

  /* current leaf used to move in the tree */
  current = first;

  #ifdef DEBUG
    n_lvl = 4; //NCELL*NCELL;
  #else
    n_lvl = NCELL*NCELL;
  #endif
  
  int n_leaf_2_current;
  
  /* for all lvl */
  for(i=0; i<n_lvl-1; i++){
    
    #ifdef DEBUG
      printf("---> working on lvl %d - number of leafs %d\n", i, leaf_by_lvl[i]);
    #endif
    
    /* for all leaf in the lvl */
    for(j=0; j<leaf_by_lvl[i]; j++ ){
      current->next = (Node **) malloc(leaf_by_lvl[i+1]*sizeof(Node **));
      current->n_subnode = leaf_by_lvl[i+1];
	
      #ifdef DEBUG
	printf("\t---> Leaf %d \n",j);
	printf("\t\t---> Address %p \n", current);
	printf("\t\t---> Address array of next %p \n", current->next);
	printf("\t\t---> Number of leafs %d\n", current->n_subnode);
      #endif
	    
	for(k=0; k<leaf_by_lvl[i+1]; k++){
	  current->next[k] = (Node *) malloc(sizeof(Node *));
	  (current->next[k])->previous = current;

	  #ifdef DEBUG
	    printf("\t\t\t---> Leaf address %p \n",current->next[k]);
	  #endif

	  
	}
	
	/* dirty ... */
	if( current->previous )
	  current = (current->previous)->next[j];
	else
	  current = current->next[j]; 
      }
          
  }

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
