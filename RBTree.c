#include "Structs.h"
#include "RBTree.h"
#include "stdio.h"
#include "stdlib.h"

int forEach(Node *node, forEachFunc func, void *args, int temp);
static void insertHelper(RBTree *root, Node *z);

static void leftRotate(RBTree *root, Node *z);

static void rightRotate(RBTree *root, Node *z);

static Node *searchNode(RBTree root, void *key);

static void __rb_transplant(RBTree *root, Node *u, Node *v);

static void deleteHelper(RBTree *root, Node *x);

static Node *getMin(Node *z);

int forEach(Node *node, forEachFunc func, void *args, int temp);

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc){
    RBTree *z = malloc(sizeof(Node) * 50 + sizeof(CompareFunc) + sizeof(FreeFunc));
    z->size = 0;
    z->freeFunc = freeFunc;
    z->compFunc = compFunc;
    z->root = NULL;
    return z;
}

static Node *newNode(void *data)
{
    Node *node1 = malloc(sizeof(Node));

    node1->data = data;
    node1->color = RED;
    node1->right = NULL;
    node1->left = NULL;
    node1->parent = NULL;

    return node1;
}


/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data){//not mine

    if(data==NULL || tree==NULL){
        return 0;
    }
    Node *y = NULL;
    Node *x = tree->root;

    void **Pdata = data;
    Node *z = newNode(Pdata);

    while (x != NULL)
    {
        y = x;

        if (tree->compFunc(x->data, z->data) == 0)
        {
            free(z);
            x->data = data;
            return 0;
        }
        // (tree->compFunc(x->data, z->data) > 0) ? (x->left = z) : (x->right = z);
        (tree->compFunc(z->data, x->data) < 0) ? (x = x->left) : (x = x->right);
    }
    z->parent = y;

    if (y == NULL)
    {
        tree->root = z;
    }

    else
    {
        (tree->compFunc(z->data, y->data) < 0) ? (y->left = z) : (y->right = z);
    }
    insertHelper(tree, z);
    tree->size++;
    //    if (tree->size % 10 == 0)
    //    {
    //        tree = realloc(tree, sizeof(Node) * (tree->size + 10));
    //    }
    return 1;

} // implement it in RBTree.c


/**
 *
 * @param root
 * @param z the new node
 */
static void insertHelper(RBTree *root, Node *z){
    Node *y;
    if(z->parent==NULL){
        root->root->color=BLACK;
        return;
    }
    while(z->parent->color==RED){
        if(z->parent==z->parent->parent->left){
          y=z->parent->parent->right;
          if(y!= NULL && y->color==RED){
              z->parent->color=BLACK;
              y->color=BLACK;
              z->parent->parent->color=RED;
              z=z->parent->parent;
          }
          else if(z==z->parent->right){
              z=z->parent;
              leftRotate(root,z);//z has red father
          }
          if(z->parent != NULL){
              z->parent->color=BLACK;
              z->parent->parent->color=RED;
              rightRotate(root,z->parent->parent);
          }

        }

        else if(z->parent->parent != NULL && z->parent==z->parent->parent->right){//the same
            y=z->parent->parent->left;
            if(y->color==RED){
                z->parent->color=BLACK;
                y->color=BLACK;
                z->parent->parent->color=RED;
                z=z->parent->parent;
            }
            else if(z->parent->left !=NULL && z->parent->left==z){
                z=z->parent;
                rightRotate(root,z);
            }
            z->parent->color=BLACK;
            z->parent->parent->color=RED;
            leftRotate(root,z->parent->parent);
        }
    }
    root->root->color=BLACK;
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data); // implement it in RBTree.c

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data){
    if(data==NULL || tree==NULL){
        return 0;
    }
    Node *z = tree->root;

    while (z != NULL)
    {
        if (tree->compFunc(z->data, data) == 0)
        {
            return 1;
        }
        (tree->compFunc(z->data, data) > 0) ? (z = z->left) : (z = z->right);
    }

    return 0;
} // implement it in RBTree.c



/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args){
    if(tree==NULL || func==NULL){
        return 0;
    }
    int temp = 1;
    return forEach(tree->root, func, args, temp);
} // implement it in RBTree.c

int forEach(Node *node, forEachFunc func, void *args, int temp)
{

    if (node != NULL)
    {
        forEach(node->left, func, args, temp);

        if (func(node->data, args) == 0)
        {
            temp = 0;
        }
        forEach(node->right, func, args, temp);

    }
    return temp;
}

void freeTree(Node *node)
{
    if(node==NULL){
        return;
    }
    if (node->right != NULL)
    {
        freeTree(node->right);
    }
    if (node->left != NULL)
    {
        freeTree(node->left);
    }
    free(node);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree){
    freeTree((*tree)->root);
} // implement it in RBTree.c



static Node *searchNode(RBTree root, void *key)
{
    Node *z = root.root;

    while (z != NULL)
    {
        if (root.compFunc(z->data, key) == 0)
            return z;

        (root.compFunc(z->data, key) < 0) ? (z = z->right) : (z = z->left);
    }

    return NULL;
}

static Node *getMin(Node *z)
{
    for (; z->left != NULL; z = z->left);
    return z;
}