/***************************************************************************************************
 * Description: Linked list library example - source code.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 14/02/2013
 **************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "linked_listStructs.h"


int list_add_first(st_list *list, st_list_item *item)
{
	if (!list || !item) {
		return -1;
	}

	item->next = NULL;
	item->index = 0;

	list->first = item;
	list->last = item;
	list->item_counter = 1;

	return 0;
}

int list_add_next(st_list *list, st_list_item *item)
{
	if (!list || !item) {
		return -1;
	}

	list->last->next = item;
	list->last = item;
	item->index = list->item_counter;

	list->item_counter++;

	return 0;
}

st_list_item *list_get_first(st_list *mlist)
{
	if (!mlist) {
		return NULL;
	}
	else {
		return mlist->first;
	}
}

st_list_item *list_get_last(st_list *mlist)
{
	if (!mlist) {
		return NULL;
	}
	else {
		return mlist->last;
	}
}

st_list_item *list_get_item(st_list *mlist, unsigned int index)
{
	st_list_item *item = NULL;

	if ( (!mlist) || (index > mlist->item_counter)) {
		return NULL;
	}

	if (index == (mlist->item_counter - 1)) {
		return mlist->last;
	}

	item = mlist->first;
	while (item) {
		if (item->index == index) {
			return item;
		}
		item = item->next;
	}

	return NULL;
}

void list_destroy(st_list **mlist)
{
	st_list_item *cur = NULL;
	st_list_item *after = NULL;

	if (*mlist == NULL) {
		return;
	}

   if ((**mlist).first == NULL) {
		free(*mlist);
		*mlist = NULL;
	}

	after = (**mlist).first->next;

	if ((**mlist).first->data != NULL) {
		free((**mlist).first->data);
	}
	free((**mlist).first);

	while(after) {
		cur = after;
		after = cur->next;
		if (cur->data != NULL) {
			free(cur->data);
		}
		free(cur);
	}

	free(*mlist);
	*mlist = NULL;
}

