#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "list-new.h"

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
		p->str = str;
		list_add_tail(&p->list, list);
	}
}

static void free_entry(struct list_head *entry)
{
	free(list_entry(entry, struct entry, list));
}

int main(int argc, char **argv, char **env)
{
	LIST_HEAD(list);
	int i = 0;

	while (*++argv)
		make_entry(i++, *argv, &list);

	say_list(&list);

	return 0;
}
