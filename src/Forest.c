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
  int i, j, k, n_lvl, n_current, tmp, total_node=0;
  int nleaf_lvl[NCELL*NCELL];

   /* contains all node to dispatche trouhght the tree
   * alco could be used to parcours l'arbre en largeur 
   */
  Node ***leafs; 
  Node *current_leaf;
  
  /* pointor to the tree */
  Tree *tree;
  
  /* number of level of the tree */
  n_lvl = NCELL*NCELL;

  /* test allocation the tree =) */
  tree = (Tree *) malloc(sizeof(Tree *));
  if( !tree ){
    printf("Allocation of tree failed !");
    return NULL;
  }else
    printf("> Allocation of Tree success - mem adresse %p\n", tree);

  /* compute leaf by level ----------------------------------------------- CHECK OK */
  /* level 0 -> 1 leaf
   * level 1 -> 8 leafs per 1 mother leaf
   * level 2 -> 56 leafs 7 per mother leaf
   * level 3 -> 336 leafs 6 per mother leaf ...
   */
  nleaf_lvl[0] = 1;
  tmp = NCELL*NCELL-1;
  total_node = nleaf_lvl[0];
  for(i=1; i<n_lvl; i++){
    nleaf_lvl[i] = nleaf_lvl[i-1]*tmp;
    total_node += nleaf_lvl[i];
    tmp --;
  }
  /* --------------------------------------------------------------------- ----- -- */
  
  /* -------------------------------------- allocate matrix -------------- CHECK OK */
  
  /* Allocate all nodes in triangular like matrix  
   * allocate next array, set n_subnode value
   */
  leafs = (Node ***) malloc(total_node*sizeof(Node **));
  if( !leafs ){
    printf("Allocation of leafs failed ! \n");
    return NULL;
  }else
    printf("> Allocation of *** leafs success - mem adresse %p\n", leafs);
    
  for(i=0; i<n_lvl; i++){
    leafs[i] = (Node **) malloc(nleaf_lvl[i]*sizeof(Node *));
    if( leafs[i] == NULL ){
      printf("Allocation of **leafs[%d] failed ! \n",i);
      return NULL;
    }else
      printf("> Allocation of **leafs[%d] success - mem adresse %p\n", i, leafs[i]);
  }
  
  /* --------------------------------------------------------------------- ----- -- */
  
  /* ------------------------------------ allocate node in matrix -------- CHECK OK */
  for(i=0; i<n_lvl; i++){
   for(j=0; j<nleaf_lvl[i]; j++){
    leafs[i][j] = (Node *) malloc(sizeof(Node));
    if( leafs[i][j] == NULL ){
      printf("Allocation of *leafs[%d][%d] failed ! \n",i,j);
      return NULL;
    }
   }
  }
  /* --------------------------------------------------------------------- ----- -- */
  
  
/*  #ifdef DEBUG
  printf("level %d  with %d leafs -> this %d @ %p\n",0, 1, 0,leafs[0][0]);
  printf(" ---------------------------------------------------------\n");
  for(i=1; i<n_lvl; i++){
    n_2_alloc = leaf_by_lvl[i-1] * leaf_by_lvl[i];
    for(j=0; j<n_2_alloc; j++ ){
      printf("level %d  with %d leafs -> this %d @ %p\n",i, n_2_alloc, j, leafs[i][j]);
    }
    printf(" ---------------------------------------------------------\n");
  }
  #endif*/
  
  /* loop over the leafs array to dispach all nodes in next arrays */
/*  n_current = n_lvl-1;
  tree->root = leafs[0][0];
  
  #ifdef DEBUG
    char tmp2[10]={""};
    tmp2[9] = '\0';
  #endif
  
  current_leaf = leafs[0][0];
  for(i=1; i<n_lvl-1; i++){
    
    for(j=0; j<leaf_by_lvl[i-1]; j++){
      
      #ifdef DEBUG
        printf("%s|-> leaf %d @ at %p with %d subleafs \n", tmp2, i, current_leaf, leaf_by_lvl[i]);
      #endif
      
      for(k=0; k<leaf_by_lvl[i]; k++){
        #ifdef DEBUG
          printf("\t%s --> subleaf @ %p\n", tmp2, leafs[i][j*leaf_by_lvl[i-1]+k]);
        #endif
        
          current_leaf->next[k] = leafs[i][j*leaf_by_lvl[i-1]+k];
      }
      
      current_leaf = leafs[i][j];
    }
    
    #ifdef DEBUG
      tmp2[i]= '\t';
    #endif
  } */
  
  return tree;
}

/*
 * Display the tree - used for debugging purpose !
 *
 * Parameters :
 *              Tree *bonzai : pointor to the tree to display
 */
void show_tree(Node *current_leaf){
  
  int i, n_lvl = NCELL*NCELL;
  char tmp[10]={""};
    
  tmp[9] = '\0';
  for(i=0;i<current_leaf->n_subnode; i++){
      printf("%s|-> leafs %p with subleafs %p \n", tmp, current_leaf, current_leaf->next[i] );
  }
  
  if( current_leaf->next != NULL )
    show_tree(current_leaf->next[0]);
  else
    return;
}
