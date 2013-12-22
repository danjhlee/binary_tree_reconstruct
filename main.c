#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUESIZE   10

/*
 * Binary Tree Data Structure
 */
typedef struct tree
{
    char letter;
    struct tree *left;
    struct tree *right;
} tree;

/*
 * Queue Data Structure
 */
typedef struct queue
{
    char q[QUEUESIZE+1];
    int first;
    int last;
    int count;
} queue;

queue *q;

/*
 * initialize queue
 */
void init_queue(queue *q)
{
    q->first = 0;
    q->last = QUEUESIZE - 1;
    q->count = 0;
}

/*
 *  enqueue a character into queue
 */
void enqueue(queue *q, char x)
{
    if (q->count >= QUEUESIZE)
        printf("ERROR: queue overflow enqueue x = %c\n", x);
    else
    {
        q->last = (q->last+1) % QUEUESIZE;
        q->q[q->last] = x;
        q->count = q->count + 1;
    }
}

/*
 * dequeue a character from queue
 */
char dequeue(queue *q)
{
    char x;

    if (q->count <= 0)
        printf("ERROR: empty queue dequeue\n");
    else
    {
        x = q->q[q->first];
        q->first = (q->first + 1) % QUEUESIZE;
        q->count = q->count - 1;
    }
    return x;
}

/*
 * print the whole items in queue
 */
void print_queue(queue *q)
{
    int i;

    for (i = q->first; i <= q->count; i++)
    {
        printf(" %c ", q->q[i]);
    }
}

/*
 * preorder traversal of binary tree
 */
void preorder_traversal(tree *node)
{
    if (node == NULL)
        return;

    printf("%c->", node->letter);
    /* put node item to a queue */
    enqueue(q, node->letter);
    preorder_traversal(node->left);
    preorder_traversal(node->right);
}

/*
 * inorder traversal of binary tree
 */
void inorder_traversal(tree *node)
{
    if (node == NULL)
        return;

    inorder_traversal(node->left);
    printf("%c->", node->letter);
    /* put node item to a queue */
    enqueue(q, node->letter);
    inorder_traversal(node->right);
}

/*
 * postorder traversal of binary tree
 */
void postorder_traversal(tree *node)
{
    if (node == NULL)
        return;

    postorder_traversal(node->left);
    postorder_traversal(node->right);
    printf("%c->", node->letter);
    /* put node item to a queue */
    enqueue(q, node->letter);
}

/*
 * return the number of nodes in a tree
 */
int get_num_nodes(tree *node)
{
    int count = 0;

    if (node != NULL)
        count = 1 + get_num_nodes(node->left) + get_num_nodes(node->right);

    return count;
}

/*
 * create a new binary tree node
 */
tree *new_node(char ch)
{
    tree *node;

    node = (tree *)calloc(1, sizeof(tree));
    node->letter = ch;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/*
 * search the index of a character in the inorder sequence
 */
int search(char *buf, int start, int end, char value)
{
    int i;

    for (i=start; i<=end; i++)
    {
        if (*(buf + i) == value)
        {
            return i;
        }
    }
    return -1;
}

/*
 * build a binary tree recursively using inorder and preorder sequences
 *
 * In the preorder sequence, the first character (*preorder) is the root
 * of the binary tree. In the inorder sequence, all the left items of the
 * root are in the left subtree and all the right items of the root are in
 * the right subtree. When we do perform the above steps recursively, we can
 * reconstruct a binary tree.
 */
tree *build_tree(char *inorder, char *preorder, int start, int end)
{
    static int preorder_index = 0;
    int inorder_index;
    tree *node;

    if (start > end)
        return NULL;
    node = new_node(*(preorder + preorder_index));
    ++preorder_index;

    if (start == end)
        return node;
    inorder_index = search(inorder, start, end, node->letter);

    node->left = build_tree(inorder, preorder, start, inorder_index -1);
    node->right = build_tree(inorder, preorder, inorder_index + 1, end);

    return node;
}

int main()
{
    tree *root, *root_new;
    int n, i;
    char *preorder, *inorder;
    char *a, *b;

    n = 0;
    root = new_node('a');
    root->left = new_node('b');
    root->right = new_node('c');
    root->left->left = new_node('d');
    root->left->right = new_node('e');
    root->right->left = new_node('f');
    root->right->right = new_node('g');

    n = get_num_nodes(root);
    printf("number of nodes: %d\n", n);

    q = (queue *)calloc(1, sizeof(queue));
    init_queue(q);

    a = (char *)calloc(n+1, sizeof(char));
    b = (char *)calloc(n+1, sizeof(char));
    preorder = a;
    inorder = b;

    printf("\npreorder traversal: ");
    preorder_traversal(root);
    for (i = 0; i < n; i++)
    {
        *a++ = dequeue(q);
    }
    *a = '\0';
    printf("\npreorder squence : %s\n", preorder);

    printf("\ninorder traversal: ");
    inorder_traversal(root);
    for (i = 0; i < n; i++)
    {
        *b++ = dequeue(q);
    }
    *b = '\0';
    printf("\ninorder squence : %s\n", inorder);

    printf("\nreconstructed binary tree");
    root_new = build_tree(inorder, preorder, 0, strlen(preorder) - 1);
    printf("\ninorder traversal: ");
    inorder_traversal(root_new);

    return 0;
}
