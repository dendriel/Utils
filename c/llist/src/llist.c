/***************************************************************************************************
 * Description: Linked list example - source code.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 14/02/2013
 **************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"
#include "llist_defines.h"
#include "llist_structs.h"


/*************************************************************************************************/

en_llist_ret_code llist_init(st_list **list)
{
	*list = (st_list *)malloc(sizeof(st_list));
	if (*list == NULL) {
		return LLIST_RET_ERROR;
	}

	memset(*list, 0, sizeof(st_list));

	return LLIST_RET_SUCCESS;
}

/*************************************************************************************************/

en_llist_ret_code llist_add_first(st_list *list, st_list_item *item)
{
	if (!list || !item) {
		return LLIST_RET_ERROR;
	}

	list->item_index = 0;

	item->next = NULL;
	item->prev = NULL;
	item->index = list->item_index;

	list->first = item;
	list->last = item;
	list->item_counter = 1;
	list->item_index++;

	return LLIST_RET_SUCCESS;
}

/*************************************************************************************************/

en_llist_ret_code llist_add_next(st_list *list, st_list_item *item)
{
	if (!list || !item) {
		return LLIST_RET_ERROR;
	}

	st_list_item *prev = NULL;

/*	printf("\nlast: %p; last->next: %p\nfirst: %p; first->next: %p",
		 list->last, list->last->next, list->first, list->first->next);*/

	/* Save [current last item], that will be the previous of the new last. */
	prev = list->last;
	/* Save the [new item] as [next item] of the [current last item]. */
	list->last->next = item;
	/* Save the [new item] as [last item]. */
	list->last = item;
	/* Save the [old last item] as previous of the [new last item]. */
	list->last->prev = prev;
	/* Clean the "next" item for the last element so we can use this value as control. */
	list->last->next = NULL;
	/* Save item counter as index reference for the new item. */
	item->index = list->item_index;

/*	printf("\nlast: %p; last->next: %p\nfirst: %p; first->next: %p",
		 list->last, list->last->next, list->first, list->first->next);*/

	list->item_counter++;
	list->item_index++;

	return LLIST_RET_SUCCESS;
}

/*************************************************************************************************/

en_llist_ret_code llist_rm_index(st_list *list, const unsigned int index)
{
	st_list_item *item = NULL;
	st_list_item *item_next = NULL;
	st_list_item *item_prev = NULL;

	item = llist_get_item(list, index);
	if (item == NULL) {
		return LLIST_RET_NOTFOUND;
	}

	item_next = item->next;
	item_prev = item->prev;

	/* Remove first item. A<->B<->C => B<->C */
	if (item == list->first) {
		item_next->prev = NULL;
		list->first = item_next;
	}
	/* Remove last item. A<->B<->C => A<->B */
	else if (item == list->last) {
		item_prev->next = NULL;
		list->last = item_prev;
	}
	/* Remove middle item. A<->B<->C => A<->C */
	else {
		item_prev->next = item_next;
		item_next->prev = item_prev;
	}

	list->item_counter--;

	free(item->data);
	free(item);

	return LLIST_RET_SUCCESS;
}

/*************************************************************************************************/

st_list_item *llist_get_first(st_list *list)
{
	if (!list) {
		return NULL;
	}
	else {
		return list->first;
	}
}

/*************************************************************************************************/

st_list_item *llist_get_last(st_list *list)
{
	if (!list) {
		return NULL;
	}
	else {
		return list->last;
	}
}

/*************************************************************************************************/

st_list_item *llist_get_next(st_list *list, unsigned int *index)
{
	if (!list && !index) {
		return NULL;
	}

	st_list_item *item = NULL;

	/* List already ended. */
	if (*index == -1) {
		return NULL;
	}

	/* If want to start by the first item. */
	if (*index == 0) {
		item = list->first;
	}
	else {
		item = llist_get_item(list, *index);
	}

	if (item == NULL) {
		return NULL;
	}

	/* Save reference to iteract later if needed. */
	if (item->next == NULL) {
		*index = -1; // I can't do that =[
	}
	else {
		*index = item->next->index;
	}

	return item;
}

/*************************************************************************************************/

st_list_item *llist_get_item(st_list *list, unsigned int index)
{
	if (!list) {
		return NULL;
	}

	st_list_item *item = NULL;

	item = list->first;
	while (item) {
		if (item->index == index) {
			return item;
		}
		item = item->next;
	}

	return NULL;
}

/*************************************************************************************************/

void llist_free_items(st_list **list)
{
	if(list == NULL) {
		return;
	}

	if((*list)->item_counter == 0) {
		free(*list);
		*list = NULL;
		return;
	}

	st_list_item *cur = NULL;
	st_list_item *after = NULL;

	cur = (*list)->first;
	after = cur->next;
	while (cur) {
		free(cur->data);
		free(cur);
		cur = after;
		after = (cur != NULL)? cur->next : NULL;
	}
}

/*************************************************************************************************/

void llist_destroy(st_list **list)
{
	llist_free_items(list);
	free(*list);
	*list = NULL;
}

