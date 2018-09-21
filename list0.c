#include <stdio.h>
#include <malloc.h>
#include "list-new-ref.h"

int main(int argc, char **argv, char **env)
{
	struct xyz { int val; struct list_head list; } *p, *t;
	LIST_HEAD(list1);
	LIST_HEAD(list2);
	int i;

	for (i = 0; i < 10; i++) {
		p = malloc(sizeof(*p));
		if (!p)
			break;
		p->val = i;
		list_add_tail(&p->list, &list1);
	}
	list_swap(list1.next, list1.prev);
	list_swap(list2.next, list2.prev);
	list_for_each_entry_safe(p, t, &list1, list) {
		printf("%2d\n", p->val);
		list_move(&p->list, &list2);
	}
	if (!list_empty(&list1))
		printf("List \"list1\" not empty!\n");
	printf("~~~~\n");
	i = 4;
	list_for_each_entry_safe(p, t, &list2, list) {
		if (!i--)
			break;
	}
	list_cut(&list1, &list2, &p->list);
	list_for_each_entry_safe(p, t, &list1, list) {
		printf("%2d\n", p->val);
	}
	printf("----\n");
	list_for_each_entry_safe(p, t, &list2, list) {
		printf("%2d\n", p->val);
	}
	printf("~~~~\n");
	list_move(list1.prev, &list2);
	i = 2;
	list_for_each_entry_safe(p, t, &list1, list) {
		if (!i--)
			break;
	}
	list_del(&p->list);
	list_replace(list1.next, &p->list);
	list_swap(list2.prev, list1.prev);
	list_for_each_entry_safe(p, t, &list2, list) {
		printf("%2d\n", p->val);
	}
	printf("----\n");
	list_for_each_entry_safe(p, t, &list1, list) {
		printf("%2d\n", p->val);
	}
	printf("====\n");
	list_splice(&list1, &list2);
	list_for_each_entry_safe(p, t, &list2, list) {
		printf("%2d\n", p->val);
		list_del(&p->list);
		free(p);
	}
	printf("----\n");
	if (!list_empty(&list2))
		printf("List \"list2\" not empty!\n");
	if (!list_empty(&list1))
		printf("List \"list1\" not empty!\n");

	return 0;
}
