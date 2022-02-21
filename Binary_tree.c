#include <stdio.h>
#include <stdlib.h>

typedef int Data;

struct Node {
    Data val;
    struct Node * left;
    struct Node * right;
};

struct Node * root;

void print_tree_dfs(struct Node * tree)
{
	if (tree == NULL)
    {
		return;
	}
	print_tree_dfs(tree->left);
	printf("%d", tree->val);
	print_tree_dfs(tree->right);
	return;
}

#define max_N_node 100
struct Node * queue[max_N_node]; // выделим массив, который хранит адреса вершин
int i_beg = 0, i_end = 0, i_layer = 0;

void add_to_queue(struct Node * p)
{ // добавить указатель на вершину дерева в массив
	queue[i_end] = p; // положить в конец
	i_end++;
	return;
}
struct Node * take_from_queue()
{
	if(i_beg == i_layer)
	{ // это чтобы разбить по уровням дерева
		printf("\n"); // прежде чем вынуть последний элемент уровня, поставим "\n", отметим конец следующего уровня
		i_layer = i_end;
	}
	struct Node * q = queue[i_beg]; // запомнить значение, чтобы потом сделать return
	i_beg++; // увеличить индекс начала очереди (i_beg - индекс первого из необработанных элементов) 
	return q;
}
int queue_not_empty()
{
	if(i_beg != i_end)
	{
		return 1;
	}
	else
	{
		return 0;
	}	
}

void print_tree_bfs(struct Node * tree)
{
	struct Node * q;

	add_to_queue(tree); // создадим очередь
	while(queue_not_empty())
	{ // пока в ней есть элементы
		q = take_from_queue(); // вынем элемент из очереди
		printf("%d ", q->val);
		if(q->left != NULL)
		{
			add_to_queue(q->left);
		}
		if(q->right != NULL)
		{
			add_to_queue(q->right);
		}
	}
	return;
}

void add_node(struct Node * tree, int x)
{
	struct Node * p = (struct Node *)malloc(sizeof(struct Node));
	p->val = x;
	p->left = NULL;
	p->right = NULL;

	if (tree == NULL)
    {
		root = p;
		return;
	}
	
	struct Node * q = tree;
	
	if (x < q->val)
    {
		if (q->left != NULL)
        {
            add_node(q->left, x);
			return;
		}
		else
        {
			q->left = p;
		}
	}
	else
	{
		if (q->right != NULL)
		{
			add_node(q->right, x);
			return;
		}
		else
		{
			q->right = p;
		}
	}
	return;
}

void find_node(struct Node * tree, int x)
{
	if (tree == NULL)
    {
		printf("Empty tree\n");
		return;
	}

	struct Node * p = tree;

	printf("Searching... (%d) ", p->val);

	if (x == p->val)
    {
		printf("\nFound this node!");
		print_tree_bfs(p);
		return;
	}
	else if(x < p->val)
	{
		if (p->left != NULL)
        {
            find_node(p->left, x);
		}
		else
		{
			return;
		}
	}
	else
	{
		if (p->right != NULL)
        {
            find_node(p->right, x);
		}
		else
		{
			return;
		}
	}
	return;
}

struct Node * right_descendant(struct Node * tree, struct Node * original)
{
	struct Node * p;
	if(tree == original)
	{
		p = tree->left;
	}
	else
	{
		p = tree;
	}

	if(p->right == NULL)
	{
		return p;
	}

	if(p->right->right == NULL)
	{
		return p;
	}

	return right_descendant(p->right, original);
}

void remove_node(struct Node * tree, int x)
{
	if (tree == NULL)
    {
		printf("Empty tree\n");
		return;
	}

	struct Node * p = tree;

	printf("Searching for victim... (%d) ", p->val);

	if(p->left != NULL)
	{
		if (x == p->left->val)
    	{
			printf("\nFound the victim!");
			print_tree_bfs(p->left);
			printf("\n(Victim is on the left) ");
			if(p->left->left != NULL)
			{
				struct Node * right_desc_parent = right_descendant(p->left, p->left);
				struct Node * right_desc;
				if(right_desc_parent->right != NULL)
				{
					right_desc = right_desc_parent->right;
				}
				else
				{
					right_desc = right_desc_parent;
				}
				printf("\nFurthest right descendant is %d, and his parent is %d", right_desc->val, right_desc_parent->val);
				//Заменяем ссылку на заменяющий элемент у его родителя
				right_desc_parent->right = right_desc->left;
				//Присваиваем крайнему правому потомку ссылки на потомков удаляемого:
				right_desc->right = p->left->right;
				if(p->left->left != right_desc)
				{
					right_desc->left = p->left->left;
				}
				else
				{
					right_desc->left = NULL;
				}
				//Обнуляем ссылки на потомков у удаляемого:
				p->left->left = NULL;
				p->left->right = NULL;
				free(p->left);
				//Делаем крайнего правого потомка потомком родителя удаляемого (ставим на место удаляемого):
				p->left = right_desc;
			}
			else
			{
				printf("It has no left descendants");
				p->left = p->left->right;
			}

			return;
		}
	}
	
	if(p->right != NULL)
	{
		if(x == p->right->val)
		{
			printf("\nFound the victim!");
			print_tree_bfs(p->right);
			printf("\n(Victim is on the right) ");
			
			if(p->right->left != NULL)
			{
				struct Node * right_desc_parent = right_descendant(p->right, p->right);
				struct Node * right_desc;
				if(right_desc_parent->right != NULL)
				{
					right_desc = right_desc_parent->right;
				}
				else
				{
					right_desc = right_desc_parent;
				}
				printf("\nFurthest right descendant is %d, and his parent is %d", right_desc->val, right_desc_parent->val);

				right_desc_parent->right = right_desc->left;

				right_desc->right = p->right->right;
				if(p->right->left != right_desc)
				{
					right_desc->left = p->right->left;
				}
				else
				{
					right_desc->left = NULL;
				}
				
				p->right->left = NULL;
				p->right->right = NULL;
				free(p->right);

				p->right = right_desc;
			}
			else
			{
				printf("It has no left descendants");
				p->right = p->right->right;
			}

			return;
		}
	}

	if(x < p->val)
	{
		if (p->left != NULL)
        {
            remove_node(p->left, x);
		}
		else
		{
			return;
		}
	}
	else
	{
		if (p->right != NULL)
        {
            remove_node(p->right, x);
		}
		else
		{
			return;
		}
	}
	return;
}

int leaves = 0;

void find_leaf(struct Node * tree)
{
	if (tree == NULL)
    {
		return;
	}

	struct Node * p = tree;

	if(p->left == NULL && p->right == NULL)
	{
		printf("Leaf value: %d\n", p->val);
		leaves++;
	}

	else if(p->left != NULL)
	{
		find_leaf(p->left);
	}
	
	find_leaf(p->right);
	return;
}

int main()
{
    root = NULL;
	int num;
	printf("Insert number of nodes: ");
	scanf("%d", &num);
	printf("Insert nodes: ");
	int * nodes = (int *)malloc(num * sizeof(int));
	for(int i = 0; i < num; i++)
	{
		scanf("%d", &nodes[i]);
	}
	for(int i = 0; i < num; i++)
	{
		add_node(root, nodes[i]);
	}

	printf("Depth:\n");
	print_tree_dfs(root);

	printf("\nBreadth:");
	print_tree_bfs(root);

	printf("\n\nFinding node: 8\n");
	find_node(root, 8);

	printf("\n\nFinding leaves:\n");
	find_leaf(root);
	printf("Number of leaves: %d\n", leaves);


	printf("\nChoose the victim! ");
	int victim;
	scanf("%d", &victim);
	printf("Deleting element %d\n", victim);
	remove_node(root, victim);

	printf("\nModified tree:");
	print_tree_bfs(root);

	free(nodes);
	return 0;
}