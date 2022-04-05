#include <stdio.h>
#include <stdlib.h>

#define max_N_node 100

struct Node {
	int val;
	struct Node * left;
	struct Node * right;
};

struct Node * root;

struct Remove {
	struct Node * victim;
	struct Node * sub;
	struct Node ** link;
	struct Node ** link_to_sub;
};

void add_node(struct Node * tree, int x)
{
	struct Node * p = (struct Node *)malloc(sizeof(struct Node));
	p->val = x;
	p->left = NULL;
	p->right = NULL;

	if(tree == NULL)
    {
		root = p;
		return;
	}
	
	struct Node * q = tree;
	
	if(x < q->val)
    {
		if(q->left != NULL)
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
		if(q->right != NULL)
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

struct Node * queue[max_N_node];
int i_beg = 0, i_end = 0, i_layer = 0;

void add_to_queue(struct Node * p)
{
	queue[i_end] = p;
	i_end++;
	return;
}
struct Node * take_from_queue()
{
	if(i_beg == i_layer)
	{
		printf("\n");
		i_layer = i_end;
	}
	struct Node * q = queue[i_beg];
	i_beg++;
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

	add_to_queue(tree);
	while(queue_not_empty())
	{
		q = take_from_queue();
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

struct Node * go_down_right(struct Node * goer)
{
	// Просто идёт направо вниз, пока не встретит NULL
	if(goer->right == NULL)
	{
		return goer;
	}
	return go_down_right(goer->right);
}

void substitute(struct Remove ** info, struct Node * left)
{
	if(left == NULL)
	{
		// Если у удаляемого нет левого потомка, то так и пишем - NULL
		(*info)->sub = NULL;
		return;
	}

	// Иначе, вспомогательная функция, чтобы не заморачиваться, как я сделал в прошлый раз
	(*info)->sub = go_down_right(left);
	return;
}

struct Node ** parent_link(struct Node * tree, struct Node * target) // Поиск ссылки на элемент
{
	// Я хотел сделать эту функцию void'овой, но передумал
	struct Node * searcher = tree;
	
	if(searcher->right == target)
	{
		return &(searcher->right);
	}
	else if(searcher->left == target)
	{
		return &(searcher->left);
	}
	else if(target->val < searcher->val)
	{
		if(searcher->left != NULL)
		{
			return parent_link(searcher->left, target);
		}
	}
	else
	{
		if(searcher->right != NULL)
		{
			return parent_link(searcher->right, target);
		}
	}
}

void kill_the_victim(struct Node * tree, struct Remove ** info)
{
	// 1) Заменяем удаляемый
	if((*info)->sub == NULL)
	{
		printf("\n\nYou chose a leaf");
		// Если заменять нечем (у удаляемого нет потомков), то его надо просто удалить
		*((*info)->link) = NULL;
		free(info);
		return;
	}

	if((*info)->link == NULL)
	{
		// Если ссылки на удаляемый нет, то это значит, что этот элемент - корень
		// Заменяем его на заменямый и всё
		printf("\n\nYou chose the root");
		root = (*info)->sub;
	}
	else
	{
		// Если есть, то меняем ссылку на удаляемый
		// Без такой сложной конструкции я сделать попытался, не сработало
		*((*info)->link) = *((*info)->link_to_sub);
	}

	// 2) Заменяем ссылку с родителя заменяющего
	//printf("\n===>%p %p %d", *((*info)->link_to_sub), (*info)->sub, (*((*info)->link_to_sub))->val);
	*((*info)->link_to_sub) = (*info)->sub->left;
	// 3) Заменяем потомков заменяющего на потомков удаляемого
	(*info)->sub->left = (*info)->victim->left;
	(*info)->sub->right = (*info)->victim->right;

	free((*info)->victim);
	free(info);
	return;
}

void gather_info(struct Node * tree, int value, struct Remove ** info)
{
	if(tree == NULL)
	{
		printf("Empty tree\n");
		return;
	}

	struct Node * searcher = tree;

	printf("Searching for victim... (%d)\n", searcher->val);

	if(value == searcher->val)
	{
		printf("\nFound the victim!");
		(*info)->victim = searcher;
		// Нашли удаляемый
		
		printf("\nWe have to find the substitute...");
		substitute(info, (*info)->victim->left);
		if((*info)->sub == NULL)
		{
			(*info)->sub = (*info)->victim->right;
		}
		// Нашли заменяющий элемент
		// Для этого передали в substitute левого потомка (см. функцию)
		// Если же записался NULL, значит, что у удаляемого нет левых потомков и
		// заменяющим станет правый потомок (или NULL, если и его нет)

		printf("\nThe victim's parent...");
		(*info)->link = parent_link(root, (*info)->victim);
		// Нашли ссылку на удаляемый

		printf("\nFinally, the sub's parent...");
		(*info)->link_to_sub = parent_link((*info)->victim, (*info)->sub);
		// Нашли ссылку на заменяющий

		return;
	}
	else if(value < searcher->val)
	{
		if(searcher->left != NULL)
		{
			gather_info(searcher->left, value, info);
		}
		else
		{
			// Код ошибки
			return;
		}
	}
	else
	{
		if(searcher->right != NULL)
		{
			gather_info(searcher->right, value, info);
		}
		else
		{
			// Код ошибки
			return;
		}
	}
}

int main()
{
	int ERRORS[3] = {-1, -1, -1};

	root = NULL;
	int number_of_nodes;
	printf("Insert number of nodes: ");
	scanf("%d", &number_of_nodes);
	printf("Insert nodes: ");
	int * nodes = (int *)malloc(number_of_nodes * sizeof(int));
	for(int i = 0; i < number_of_nodes; i++)
	{
		scanf("%d", &nodes[i]);
	}
	for(int i = 0; i < number_of_nodes; i++)
	{
		add_node(root, nodes[i]);
	}

	printf("\nBreadth:");
	print_tree_bfs(root);

	printf("\nChoose the victim! ");
	int victim;
	scanf("%d", &victim);

	struct Remove * victim_info;
	gather_info(root, victim, &victim_info); // собираем инфо об удаляемом
	// printf("\n%p", victim_info->victim); - дебаг

	/*
	switch (ERRORS[0])
	{
		case 0:
			break;
		case 1:
			printf("\nThe tree is empty");
			return 0;
		default:
			printf("\nUnknown error occured upon gathering info");
			return 0;
	} */

	// Почему-то int'овые функции после return меняют адреса, или что-то типа того:
	// Я создал функцию int gather_info, аналогичную void (ctrl+c ctrl+v и чуть-чуть поменять),
	// но ничего не заработало. В ходе дебага выяснилось, что прямо перед return всё, что лежит в
	// структре victim_info, верно, т.е. хотя бы есть какие-то адреса и присвоение сработало
	// правильно; однако после возврата и печати тех же полей (в строке после вызова gather_info)
	// оказывается, что там 00000000 в поле victim. Я так и не разобрался до конца, в чём дело.
	
	kill_the_victim(root, &victim_info); // Названия функций [теперь] говорят сами за себя

	printf("\n\nModified tree:");
	print_tree_bfs(root);

	// Надо доделать очистку всего дерева

	free(nodes);
	return 0;
}
