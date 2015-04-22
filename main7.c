#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_FACTORS 64





/**
 * _______________________________structure de données de memoization : arbre binaire___________________________________
 */
 
 
 typedef struct node
{
    // la valeur du nombre à décomposer
    uint64_t key;
    // la taleau des diviseurs premiers
    uint64_t factors[MAX_FACTORS];
    // le nombre de facteurs dans le tableau
    int nbFactor;
    // les pointeurs pour la structure
    struct node *left;
    struct node *right;
} node ;

/**
 * Ajout d'un noeud correspondant à une valeur de key
 */
void addNode(node **tree, uint64_t key, uint64_t dest[], int nbFact)
{
    node *tmpNode;
    node *tmpTree = *tree;

    node *elem = malloc(sizeof(node));
    elem->key = key;
    elem->left = NULL;
    elem->right = NULL;
    elem->nbFactor = nbFact;
    int i;
   for(i=0;i<nbFact;i++)
   {
	elem->factors[i] = dest[i];
   }
    printf("nombre de facteur = %d\n", elem->nbFactor);
    printf("trace : nouveau noeud créé \n");
    if(tmpTree)
    do
    {
        tmpNode = tmpTree;
        if(key > tmpTree->key )
        {
            tmpTree = tmpTree->right;
            if(!tmpTree) tmpNode->right = elem;
        }
        else
        {
            tmpTree = tmpTree->left;
            if(!tmpTree) tmpNode->left = elem;
        }
    }
    while(tmpTree);
    else  *tree = elem;
    printf("trace : nouveau noeud ajouté \n");
}

/**
 * Recherche dans un arbre à partir d'un noeud, la valeur d'une clef
 */
node* searchNode(node *tree, uint64_t key)
{
	printf("trace : nouvelle recherche dans l'arbre \n");
    while(tree)
    {
        if(key == tree->key) return tree;

        if(key > tree->key ) tree = tree->right;
        else tree = tree->left;
    }
    return NULL;
}
/**
 * Affiche l'arbre à partir d'un certain noeurd
 */
void printTree(node *tree)
{
    
    if(!tree) return;
//	printf("trace : l'arbre est non vide \n");
    if(tree->left)  printTree(tree->left);

    printf("Cle = %d\n", tree->key);
     printf("nombre de facteurs = %d\n", tree->nbFactor);
    int j;
    for(j=0; j<tree->nbFactor; j++)
	{
		printf("%lu ",tree->factors[j]);
	}
	printf("\n");

    if(tree->right) printTree(tree->right);
    printf(" \n");
}

void printNode(node *tree)
{
    
    if(!tree) return;
    printf("Cle = %d\n", tree->key);
    printf("nombre de facteurs = %d\n", tree->nbFactor);
    int j;
    for(j=0; j<tree->nbFactor; j++)
	{
		printf("%lu ",tree->factors[j]);
	}
	printf("\n");
}

/**
 * supprime les descendant d'un noeuds
 */
void clearTree(node **tree)
{
    node *tmpTree = *tree;

    if(!tree) return;

    if(tmpTree->left)  clearTree(&tmpTree->left);

    if(tmpTree->right) clearTree(&tmpTree->right);

    free(tmpTree);	

    *tree = NULL;
}





/** _________________________________________________FIN des fonction de structure de données
 * 
 *  -> ajout à l'arbre d'une valeur : addNode(node **tree, unsigned int key, uint64_t* dest)
 *  -> test de présence dans l'arbre: if(searchNode(Arbre, Key)) 
 *  -> supprimer un noeud : clearTree(node **tree)
 *  -> affichage récursif de l'arbre : printTree(node *tree)
 * /









/**
retourne 1 si p est premier
*/
int is_prime(uint64_t p)
{
	uint64_t i;
	for(i= 2;i<(p/2);i++)
	{
		if(!(p%i))
		{
			return 0;
		}
	}
	return 1;
}

/**
 * affiche les facteurs premiers d'un nombre n
 */
void print_prime_factors(uint64_t n, node **Arbre)
{
	uint64_t factors[MAX_FACTORS];
	int j,k;
	
	
	node* cur=searchNode(*Arbre, n);
	// si le noeud n'existe pas, on le calcule le crée et onstocke le tableau des diviseurs de ce nombre n (key) sinon on l'affiche
/**	if(cur->key==n)
	{	
		printNode(cur);
	}
	else **/
	{
		k=get_prime_factors(n,factors);
		addNode(Arbre, n, factors, k);
	}
	printf("%ju: ",n);
	for(j=0; j<k; j++)
	{
		printf("%lu ",factors[j]);
	}
	printf("\n");
}

/**
 * Renvoit un tableau contenant les facteurs premiers du nombre n
 * L'algorithme effectue des tests qui ne sont effectués que au premier tour ( divisible par 2, 3, 5)
 * puis les test lors des autres tours sont par itération du pas de 2, 4, 2, 4, 2
 */
int get_prime_factors(uint64_t n,uint64_t* dest)
{
	
	uint64_t i;
	uint64_t pas=4;

	int compteur=0; // le curseur sur le tableau
	for(i=7;is_prime(n)==0;i+=(pas=6-pas))
	{	
		//les tests au premier tour
		if(i==7)
		{
			if((n)%2==0)
			{
				dest[compteur]=2;
				compteur++;
				n=n/2;
				i=5;
				pas=4;
			}
			if((n)%3==0)
			{
				dest[compteur]=3;
				compteur++;
				n=n/3;
				i=5;
				pas=4;
			}
			if((n)%5==0)
			{
				dest[compteur]=5;
				compteur++;
				n=n/5;
				i=5;
				pas=4;
			}

		}
		// les tests des autres tours
		if((n)%i==0)
		{
			dest[compteur]=i;
			compteur++;
			n=n/i;
			i=5;
			pas=4;
		}
	}
	dest[compteur]=n;
	compteur++;
	return compteur;
} 

/**
 * Le main lit dans un fichier chaque ligne et affiche les facteurs premiers
 * du nombre lu sur chaque ligne en utilisant le multi-threading
 */
int main(int argc, char *argv[])
{

	//le premier noeud: la racine de l'arbre
	node *Arbre = NULL;
	// un nombre 64 bit pivot pour ranger la ligne lue du fichier
	uint64_t p;
	int result=0;
	//création et ouverture en lecture du fichier
	FILE *fichier;
	fichier = fopen ("number.txt", "r");
	
	//test de fin de lecture du fichier
	int finish = 0;
	while (finish==0) 
	{
		if(fscanf(fichier, "%ju",&p) != EOF)
		{
			print_prime_factors(p, &Arbre);
			
		}
		else
		{
			finish = 1;
		}
	}
	//affihcage de l'arbre binaire ___TEST

	printf(" \n --- affichage de l'arbre --- \n");
	printTree(Arbre); 
	return 0;
}
