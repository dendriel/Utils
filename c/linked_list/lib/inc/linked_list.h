/***************************************************************************************************
 * Description: linked list library example - prototype of the functions.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 14/02/2013
 **************************************************************************************************/
#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include "linked_listStructs.h"

/**
 *	\b Initialize the list and add the first item.
 *	\p list Reference to the list that will be updated.
 *	\p item The element to insert into the list.
 *	\r 0 for success; -1 for error (only for invalid parameters).
 */
int list_add_first(st_list *list, st_list_item *item);

/**
 *	\b Adds a new item in the list right before the last item.
 *	\p list Reference to the list that will be updated.
 *	\p item The element to insert into the list.
 *	\r 0 for success; -1 for error (only for invalid parameters).
 */
int list_add_next(st_list *list, st_list_item *item);

/**
 *	\b Retrieve the first item from the list.
 *	\p mlist The list to retrieve the element from.
 *	\r Will return a pointer for the first item or NULL if there is not element in the list.
 */
st_list_item *list_get_first(st_list *mlist);

/**
 *	\b Retrieve the last item from the list.
 *	\p mlist The list to retrieve the element from.
 *	\r Will return a pointer for the last item or NULL if there is not element in the list.
 */
st_list_item *list_get_last(st_list *mlist);

/**
 *	\b Returns the item specified by index
 *	\p mlist The list to retrieve the element from.
 *	\p index The reference for the item inside the list.
 *	\n The way that the list is being iterated is very ugly. But
 *	this library should be used just for study purpose.
 */
st_list_item *list_get_item(st_list *mlist, unsigned int index);

/**
 *	\b Destroys the list and all his members.
 *	\p list The list itself to be freed.
 */
void list_destroy(st_list **list);

#endif /* LINKED_LIST_HEADER */
