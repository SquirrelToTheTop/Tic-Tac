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
  int i, j, k, n_lvl, tmp, total_node=0;
  int nleaf_lvl[TREE_DEPTH];

   /* contains all node to dispatche trouhght the tree
   * alco could be used to parcours l'arbre en largeur 
   */
  Node ***leafs; 
  Node *current_leaf;
  
  /* pointor to the tree */
  Tree *tree;
  
  /* number of level of the tree */
  // WARNING cut the tree for lowest memory consumption
  n_lvl = TREE_DEPTH;

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
  tmp = n_lvl-1;
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
  
  /* ------------------------------------- Set 'next' -------------------- ***** -- */
  
  // set root of the tree
  tree->root = (Node *) leafs[0][0];
  
#ifdef DEBUG
  printf("\n> Start loop over level\n");
#endif
  
  /* loop over all level */
  for(i=0; i<n_lvl-1; i++){
    
#ifdef DEBUG
  printf("\n> level %d \n",i);
#endif
    
    // loop over all leaf in one level
    for(j=0; j<nleaf_lvl[i]; j++){
      
#ifdef DEBUG
  //printf("\n\t> leaf number %d \n",j);
#endif      
      
      // allocate his array of next
      leafs[i][j]->next = (Node **) malloc(nleaf_lvl[i+1]*sizeof(Node *));
      if( leafs[i] == NULL ){
        printf("Allocation of next's array of leaf[%d][%d] failed ! \n",i,j);
        return NULL;
      }

#ifdef DEBUG
  printf("\n> Allocation succes of next @ %p \n", leafs[i][j]->next);
#endif
    
      // set his number of leaf
      leafs[i][j]->n_subnode = nleaf_lvl;
      
      // loop over all subleaf to set as next leaf
      for(k=0; k<nleaf_lvl[i+1]; k++){
        leafs[i][j]->next[k] = leafs[i+1][j*nleaf_lvl[i+1]+k];
      }
            
    }
    
  }

  /* --------------------------------------------------------------------- ----- -- */
  
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
Tree * initialize_tree_corrected_depth(){

  /* local variables */
  int i, j, k, n_lvl, tmp, count, current_nleaf, theoric_depth, total_node=0;
  int nleaf_lvl[TREE_DEPTH];

   /* contains all node to dispatche trouhght the tree
   * alco could be used to parcours l'arbre en largeur 
   */
  Node ***leafs; 
  Node *current_leaf;
  
  /* pointor to the tree */
  Tree *tree;
  
  /* number of level of the tree */
  // WARNING cut the tree for lowest memory consumption
  n_lvl = TREE_DEPTH;
  theoric_depth = (NCELL*NCELL)-1;

  /* test allocation the tree =) */
  tree = (Tree *) malloc(sizeof(Tree *));
  if( !tree ){
    printf("Allocation of tree failed !");
    return NULL;
  }else
    printf("> Allocation of Tree success - mem adresse %p\n", tree);

  /* compute leaf by level ----------------------------------------------- NEED CHECK -- */
  /* level 0 -> 1 leaf
   * level 1 -> 8 leafs per 1 mother leaf
   * level 2 -> 56 leafs 7 per mother leaf
   * level 3 -> 336 leafs 6 per mother leaf ...
   */
  nleaf_lvl[0] = 1;
  tmp = theoric_depth;
  total_node = nleaf_lvl[0];
  for(i=1; i<TREE_DEPTH; i++){
    nleaf_lvl[i] = nleaf_lvl[i-1]*tmp;
    total_node += nleaf_lvl[i];
    tmp --;
  }

#ifdef DEBUG
  for(i=0; i<TREE_DEPTH; i++)
    printf("> \t level %d with %d leafs \n", i, nleaf_lvl[i]);
  printf("\n");
#endif
  
  
  /* --------------------------------------------------------------------- ----- -- */
  
  /* -------------------------------------- allocate matrix -------------- NEED CHECK -- */
  
  /* Allocate all nodes in triangular like matrix  
   * allocate next array, set n_subnode value
   */
  leafs = (Node ***) malloc(total_node*sizeof(Node **));
  if( !leafs ){
    printf("Allocation of leafs failed ! \n");
    return NULL;
  }else
    printf("> Allocation of *** leafs success - mem adresse %p\n", leafs);
    
  for(i=0; i<TREE_DEPTH; i++){
    leafs[i] = (Node **) malloc(nleaf_lvl[i]*sizeof(Node *));
    if( leafs[i] == NULL ){
      printf("Allocation of **leafs[%d] failed ! \n",i);
      return NULL;
    }else
      printf("> Allocation of **leafs[%d] success - mem adresse %p\n", i, leafs[i]);
  }
  
  /* --------------------------------------------------------------------- ----- -- */
  
  /* ------------------------------------ allocate node in matrix -------- NEED CHECK -- */
  for(i=0; i<TREE_DEPTH; i++){
   for(j=0; j<nleaf_lvl[i]; j++){
    leafs[i][j] = (Node *) malloc(sizeof(Node));
    if( leafs[i][j] == NULL ){
      printf("Allocation of *leafs[%d][%d] failed ! \n",i,j);
      return NULL;
    }
   }
  }
  /* --------------------------------------------------------------------- ----- -- */
  
  /* ------------------------------------- Set 'next' -------------------- ***** -- */
  
  // set root of the tree
  tree->root = (Node *) leafs[0][0];
  
#ifdef DEBUG
  printf("\n> Start loop over level\n");
#endif
  
  current_nleaf = 1;
  
  /* loop over all level */
  for(i=0; i<TREE_DEPTH-1; i++){
    
#ifdef DEBUG
  printf("\n> level %d \n",i);
  tmp = 0;
  count = 0;
#endif
    
    // loop over all leaf in one level
    for(j=0; j<nleaf_lvl[i]; j++){
      
      // set his number of subleafs
      leafs[i][j]->n_subnode = (current_nleaf-1);
      
      // allocate his array of next
      leafs[i][j]->next = (Node **) malloc((current_nleaf-1)*sizeof(Node *));
      if( leafs[i][j]->next == NULL ){
        printf("Allocation of next's array of leaf[%d][%d] failed ! \n",i,j);
        return NULL;
      }
      
#ifdef DEBUG
      /* count number of allocation for this level */
      tmp ++;
#endif
      
      // loop over all subleaf to set as next leaf
      for(k=0; k<(current_nleaf-1); k++){
        leafs[i][j]->next[k] = leafs[i+1][j*(current_nleaf-1)+k];
        count ++;
      }
            
    }

    if( i == 0 )
      current_nleaf = theoric_depth;
    else
      current_nleaf --;
    
#ifdef DEBUG
  printf("\t> Allocation succes of %d 'next array' for this level\n", tmp);
  printf("\t> %d leafs has been set as subleafs of this level\n", count);
#endif
    
  }

  /* --------------------------------------------------------------------- ----- -- */
  
  return tree;
}

/*
 * Display the tree - used for debugging purpose !
 *
 * Parameters :
 *              Tree *bonzai : pointor to the tree to display
 */
void show_tree(Node *current_leaf){
  
  int i, n_lvl = TREE_DEPTH;
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
