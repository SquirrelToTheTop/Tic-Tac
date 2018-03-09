#include <stdio.h>
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
  int i, j, n_2_alloc, n_lvl;
  int leaf_by_lvl[NCELL*NCELL];

   /* contains all node to dispatche trouhght the tree
   * alco could be used to parcours l'arbre en largeur 
   */
  Node *** leafs; 
  
  /* pointor to the tree */
  Tree *tree;
  
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

  /* Allocate all nodes in triangular like matrix  
   * allocate next array, set n_subnode value
   */
  leafs = (Node ***) malloc(n_lvl*sizeof(Node **));
  for( i=1; i<n_lvl; i++){
    n_2_alloc = leaf_by_lvl[i-1] * leaf_by_lvl[i];
    
    #ifdef DEBUG
      printf("Level %d, number of leafs %d \n", i, n_2_alloc);
    #endif
    leafs[i-1] = (Node **) malloc(n_2_alloc*sizeof(Node **));
  
    for(j=0; j<n_2_alloc; j++){
      leafs[i-1][j] = (Node *) malloc(sizeof(Node *));
      leafs[i-1][j]->n_subnode = leaf_by_lvl[i];
      leafs[i-1][j]->next = (Node **) malloc(leaf_by_lvl[i]*leaf_by_lvl[i]);
    }
  }
  
  /* check and display some info */
  #ifdef DEBUG
    char tmp[10]={""};
    
    tmp[9] = '\0';
    for(i=0; i<n_lvl-1; i++){
      for(j=0;j<leaf_by_lvl[i]; j++){
        printf("%s|-> level %d, leafs %d \n", tmp, i, leafs[i][j]->n_subnode);
      }
      tmp[i] = '\t';
    }
  #endif
  
  /* loop over node in levels */
  for(i=0; i<n_lvl; i++){
    
    
  }
  
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
  int count_leaf=0;
  Node *current;

  current = bonzai->root;
  count_leaf = current->n_subnode;

  printf("\n");
  while(current->next[0] != NULL){
    printf("-----> leaf : %p having %d sub-leaf \n", current, current->n_subnode);

    if( count_leaf == current->n_subnode){
      count_leaf = 0;
      current = current->next[count_leaf];
    }else
      count_leaf ++;

  }
  
}
