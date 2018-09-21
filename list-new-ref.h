#ifndef _LIST_NEW_SPEC_H_
#define _LIST_NEW_SPEC_H_

#ifndef offsetof
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ((void *)ptr - offsetof(type, member))
#endif

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline int list_is_first(
    const struct list_head *entry, const struct list_head *head)
{
	return entry->prev == head;
}

static inline int list_is_last(
    const struct list_head *entry, const struct list_head *head)
{
	return entry->next == head;
}

static inline int list_empty(const struct list_head *head)
{
	return head == head->next;
}

static inline int list_empty_careful(const struct list_head *head)
{
	return head == head->next || head == head->prev;
}

static inline int list_at_most_singular(const struct list_head *head)
{
	return head->next == head->prev;
}

static inline int list_singular(const struct list_head *head)
{
	return !list_empty(head) && list_at_most_singular(head);
}

static inline void __list_join(
    struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void __list_init(struct list_head *entry)
{
	__list_join(entry, entry);
}

static inline void __list_add(
    struct list_head *entry, struct list_head *prev, struct list_head *next)
{
	__list_join(prev, entry);
	__list_join(entry, next);
}

static inline void list_add(
    struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head, head->next);
}

static inline void list_add_tail(
    struct list_head *entry, struct list_head *head)
{
	__list_add(entry, head->prev, head);
}

static inline void __list_del(
    struct list_head * prev, struct list_head * next)
{
	__list_join(prev, next);
}

static inline struct list_head *list_del(struct list_head *entry)
{
	if (entry->next != entry) {
		__list_del(entry->prev, entry->next);
		__list_init(entry);
		return entry;
	}
	return NULL;
}

static inline void list_replace(
    struct list_head *old, struct list_head *new)
{
	if (new != old) {
		__list_add(new, old->prev, old->next);
		__list_init(old);
	}
}

static inline void list_swap(
    struct list_head *a, struct list_head *b)
{
	struct list_head *next = a->next, *prev = b->prev;

	if (a == b || next == a->prev || b->next == prev)
		return;

	__list_join(a->prev, b);
	__list_join(a, b->next);

	if (prev == a || next == b) {
		__list_join(b, a);
	} else {
		__list_join(prev, a);
		__list_join(b, next);
	}
}

static inline void list_move(
    struct list_head *entry, struct list_head *head)
{
	if (entry != head) {
		list_del(entry);
		list_add(entry, head);
	}
}

static inline void list_move_tail(
    struct list_head *entry, struct list_head *head)
{
	if (entry != head) {
		list_del(entry);
		list_add_tail(entry, head);
	}
}

static inline void list_rotate_left(struct list_head *head)
{
	if (head->next != head->prev)
		list_move_tail(head->next, head);
}

static inline void list_rotate_right(struct list_head *head)
{
	if (head->next != head->prev)
		list_move(head->prev, head);
}

static inline void __list_cut(
    struct list_head *list, struct list_head *head, struct list_head *entry)
{
	__list_join(list, head->next);
	__list_join(head, entry->next);
	__list_join(entry, list);
}

static inline void list_cut(
    struct list_head *list, struct list_head *head, struct list_head *entry)
{
	if (list_empty(head) || entry == head)
		__list_init(list);
	else
		__list_cut(list, head, entry);
}

static inline void __list_splice(
    struct list_head *list, struct list_head *prev, struct list_head *next)
{
	__list_join(prev, list->next);
	__list_join(list->prev, next);
	__list_init(list);
}

static inline void list_splice(
    struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

static inline void list_splice_tail(
    struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)
#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; \
	     pos != (head); \
	     pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

#define list_for_each_entry(pos, head, member) \
	for (pos = list_first_entry(head, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_reverse(pos, head, member) \
	for (pos = list_last_entry(head, typeof(*pos), member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

#define list_for_each_entry_safe(pos, n, head, member) \
	for (pos = list_first_entry(head, typeof(*pos), member), \
		n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_reverse(pos, n, head, member) \
	for (pos = list_last_entry(head, typeof(*pos), member), \
		n = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_prev_entry(n, member))

#endif
