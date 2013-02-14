/*
 * brief: linked list library example - prototype functions header
 * author: vitor.rozsa
 * date: 14/02/2013
 */
#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

#include "linked_listStructs.h"

/*
 * brief Initialize the list and add the first item
 */
void list_add_first(st_list *list, st_list_item *item);

/*
 * brief Adds a new item in the list right before the last item
 */
void list_add_next(st_list *list, st_list_item *item);

/*
 * brief Returns the first item of the list, if there is one
 */
st_list_item *list_get_first(st_list *mlist);

/*
 * brief Returns the last item of the list
 */
st_list_item *list_get_last(st_list *mlist);

/*
 * brief Returns the item specified by index
 * note: the way that the list is being iterated is very ugly. But
 *  this library will serve just for study purpose.
 */
st_list_item *list_get_item(st_list *mlist, unsigned int index);

/*
 * brief Destroys the list and all his members
 */
void list_destroy(st_list **list);

#endif /* LINKED_LIST_H_INCLUDED */
