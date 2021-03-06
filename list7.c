#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "list-new-ref.h"

struct entry {
	int val;
	char *str;
	struct list_head list;
};

static void say_entry(struct list_head *entry)
{
	struct entry *p = list_entry(entry, typeof(*p), list);

	printf("%p(%d,%s,%p,%p)\n",
	    &p->list, p->val, p->str, p->list.prev, p->list.next);
}

static void say_list(struct list_head *list)
{
	struct list_head *p;

	printf("%p(%p,%p)\n", list, list->prev, list->next);
	list_for_each(p, list)
		say_entry(p);
	printf("\n");
}

static void make_entry(int val, char *str, struct list_head *list)
{
	struct entry *p = malloc(sizeof(*p));

	if (p) {
		p->val = val;
		p->str = strdup(str);
		list_add_tail(&p->list, list);
	}
}

static void free_entry(struct list_head *entry)
{
	struct entry *p = list_entry(entry, typeof(*p), list);

	free(p->str);
	free(p);
}

int main(int argc, char **argv, char **env)
{
	LIST_HEAD(list);
	LIST_HEAD(other);
	int i;

	for (i = 0; i < 10; i++)
		make_entry(i, "~", &list);
	say_list(&list);
	say_list(&other);

	list_cut(&other, &list, list.next->next->next->next);
	say_list(&list);
	say_list(&other);

	list_splice(&other, &list);
	say_list(&list);
	say_list(&other);

	list_swap(list.prev, list.next);
	say_list(&list);
	say_list(&other);

	list_move(list.prev, list.next);
	say_list(&list);
	say_list(&other);

	free_entry(list_del(list.next));
	say_list(&list);
	say_list(&other);

	list_rotate_left(&list);
	say_list(&list);
	say_list(&other);

	list_rotate_right(&list);
	say_list(&list);
	say_list(&other);

	return 0;
}
