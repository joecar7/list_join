#ifndef _LIST_NEW_H_
#define _LIST_NEW_H_

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
    const struct list_head *list, const struct list_head *head)
{
	return list->next == head;
}

static inline int list_empty(const struct list_head *head)
{
	return head == head->next;
}

static inline int list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

static inline int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_join(
    struct list_head *prev, struct list_head *next)
{
	next->prev = prev;
	prev->next = next;
}

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	__list_join(list, list);
}

static inline void __list_add(
    struct list_head *new, struct list_head *prev, struct list_head *next)
{
	__list_join(prev, new);
	__list_join(new, next);
}

static inline void list_add(
    struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(
    struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(
    struct list_head * prev, struct list_head * next)
{
	__list_join(prev, next);
}

static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}
static inline void list_replace(
    struct list_head *old, struct list_head *new)
{
	if (new != old) {
		__list_add(new, old->prev, old->next);
		INIT_LIST_HEAD(old);
	}
}

static inline void list_move(
    struct list_head *list, struct list_head *head)
{

	if (list != head) {
		__list_del_entry(list);
		list_add(list, head);
	}
}

static inline void list_move_tail(
    struct list_head *list, struct list_head *head)
{

	if (list != head) {
		__list_del_entry(list);
		list_add_tail(list, head);
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


static inline void __list_cut_position(
    struct list_head *list, struct list_head *head, struct list_head *entry)
{
	__list_join(list, head->next);
	__list_join(head, entry->next);
	__list_join(entry, list);
}

static inline void list_cut_position(
    struct list_head *list, struct list_head *head, struct list_head *entry)
{
	if (list_empty(head) || entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}

static inline void __list_splice(
    struct list_head *list, struct list_head *prev, struct list_head *next)
{
	__list_join(prev, list->next);
	__list_join(list->prev, next);
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

static inline void list_splice_init(
    struct list_head *list, struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

static inline void list_splice_tail_init(
    struct list_head *list, struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}

#define list_init	INIT_LIST_HEAD
#define list_cut	list_cut_position

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

#define list_first_entry_exists(ptr, type, member) \
	(!list_empty(ptr) ? list_first_entry(ptr, type, member) : NULL)

#define list_last_entry_exists(ptr, type, member) \
	(!list_empty(ptr) ? list_last_entry(ptr, type, member) : NULL)

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

#define list_for_each_entry_continue(pos, head, member) \
	for (pos = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_continue_reverse(pos, head, member) \
	for (pos = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = list_prev_entry(pos, member))

#define list_for_each_entry_from(pos, head, member) \
	for ( ; &pos->member != (head); \
	     pos = list_next_entry(pos, member))

#define list_for_each_entry_from_reverse(pos, head, member) \
	for ( ; &pos->member != (head); \
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

#define list_for_each_entry_safe_continue(pos, n, head, member) \
	for (pos = list_next_entry(pos, member), \
		n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_continue_reverse(pos, n, head, member) \
	for (pos = list_prev_entry(pos, member), \
		n = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_prev_entry(n, member))

#define list_for_each_entry_safe_from(pos, n, head, member) \
	for (n = list_next_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_safe_from_reverse(pos, n, head, member) \
	for (n = list_prev_entry(pos, member); \
	     &pos->member != (head); \
	     pos = n, n = list_prev_entry(n, member))

#define list_safe_reset_next(pos, n, member) \
	n = list_next_entry(pos, member)

#define list_safe_reset_prev(pos, n, member) \
	n = list_next_entry(pos, member)

#endif
