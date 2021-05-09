#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct AVL_node{
    int data;
    struct AVL_node* left;
    struct AVL_node* right;
    int height;
}AVL_NODE;

//funkcia na vytvorenie vrcholu
AVL_NODE* AVL_create(int data)
{
    AVL_NODE *out = (AVL_NODE*) malloc(sizeof(AVL_NODE));
    out->data = data;
    out->left = NULL;
    out->right = NULL;
    out->height = 1;
    return out;
}

int getMax(int a, int b)
{
    return (a > b)? a : b;

}

//pomocna funkcia na zistenie vysky vrcholu, pokial vrchol neexistuje vrati 0
int getHeight(AVL_NODE* node)
{
    if (node==NULL)
        return 0;
    return node->height;
}

//funkcia zisti vyrovnanost vrcholu
int getBalance(AVL_NODE *node)
{
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

void updateHeight(AVL_NODE* node)
{
    node->height=getMax(getHeight(node->left), getHeight(node->right)) + 1;
}

AVL_NODE* rotateRight(AVL_NODE* unbalancedHead)
{
    AVL_NODE*newHead = unbalancedHead->left;
    AVL_NODE*subTree = newHead->right;
    newHead->right = unbalancedHead;
    unbalancedHead->left = subTree;
    updateHeight(unbalancedHead);
    updateHeight(newHead);
    return newHead;
}

AVL_NODE* rotateLeft(AVL_NODE* unbalancedHead)
{
    AVL_NODE *newHead= unbalancedHead->right;
    AVL_NODE *subTree = newHead->left;
    newHead->left = unbalancedHead;
    unbalancedHead->right = subTree;
    updateHeight(unbalancedHead);
    updateHeight(newHead);
    return newHead;
}

AVL_NODE* AVL_insert(AVL_NODE *root, int data)
{
    int k = 0;
    //pokial neexistuje root, vytvori sa novy AVL_node a vrati ukazovatel nan
    if(!root) {
        root = AVL_create(data);
        return root;
    }
    AVL_NODE* pathStack[root->height]; // cesta bude dlha maximalne ako vyska korena
    AVL_NODE*iterator = root;
    //obycajne vkladanie iterativnym sposobom
    //s ukladanim cesty
    while(iterator!=NULL)
    {
        pathStack[k++] = iterator;
        if(data==iterator->data)
            return root;
        if(data<iterator->data)
            iterator = iterator->left;
        else
            iterator=iterator->right;
    }
    iterator = pathStack[--k]; //vrati sa na AVL_node, na ktorom sa bude vytvarat child
    if(data<iterator->data)
        iterator->left = AVL_create(data);
    if(data>iterator->data)
        iterator->right = AVL_create(data);
    //VYROVNAVANIE
    while(k>=0) {
        updateHeight(iterator);
        int balanceFactor = getBalance(iterator);
        //L
        if (balanceFactor > 1)
        {
            //LR
            if(getBalance(iterator->left)  == -1)
                iterator->left = rotateLeft(iterator->left);
            //LL
            //vznika novy koren
            if(root == iterator)
                return rotateRight(iterator);
            //aktualizacia rodica
            if(pathStack[--k]->right==iterator)
                pathStack[k]->right= rotateRight(iterator);
            if(pathStack[k]->left==iterator)
                pathStack[k]->left = rotateRight(iterator);
            break;
        }
        //R
        if (balanceFactor < -1)
        {
            //RL
            if(getBalance(iterator->right) == 1)
                iterator->right = rotateRight(iterator->right);
            //RR
            //vznika novy koren
            if(root == iterator)
                return rotateLeft(iterator);
            //aktualizacia rodica
            if(pathStack[--k]->right==iterator)
                pathStack[k]->right= rotateLeft(iterator);
            if(pathStack[k]->left==iterator)
                pathStack[k]->left = rotateLeft(iterator);
            break;
        }
        if(--k>=0)
            iterator = pathStack[k];
    }
    return root;
}

AVL_NODE* AVL_search(AVL_NODE* root, int key) {
    AVL_NODE*iterator = root;
    while(iterator!=NULL){
        if(iterator->data==key)
            break;
        if (key > iterator->data) {
            iterator = iterator->right;
            continue;
        }
        if (key < iterator->data)
            iterator = iterator->left;
    }
    return iterator;
}

void AVL_printTree(AVL_NODE *root, int level){
    if (root == NULL) {
        printf("\n");
    }else{
        AVL_printTree(root->right, level + 1);
        for (int i = 0; i <level*2; i++ )
            putchar('\t');
        printf ( "%d(H:%d)", root->data,root->height);
        AVL_printTree(root->left, level + 1);
    }
}

void AVL_deleteTree(AVL_NODE* node)
{
    if (node == NULL) return;
    AVL_deleteTree(node->left);
    AVL_deleteTree(node->right);
    free(node);
}

int AVL_check(AVL_NODE* root,int inserted)
{
//    double minHeight = log2(inserted + 1);  nevyuzitelne pri random cislach - nepoznam pocet vlozenych prvkov
    double maxHeight = 1.44*log2(inserted+2)-1;
    if(root->height <= maxHeight)
        return 1;
    return 0;
}
