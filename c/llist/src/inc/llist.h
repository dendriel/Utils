/***************************************************************************************************
 * Description: linked list example - prototype of the functions.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 14/02/2013
 **************************************************************************************************/
#ifndef LLIST_HEADER
#define LLIST_HEADER

#include "llist_defines.h"
#include "llist_structs.h"


/*************************************************************************************************/
/**
 *	\b Initialize the list and add the first item.
 *	\p list Reference to the list that will be updated.
 *	\p item The element to insert into the list.
 *	\r 0 for success; -1 for error (only for invalid parameters).
 */
en_llist_ret_code llist_add_first(st_list *list, st_list_item *item);

/*************************************************************************************************/
/**
 *	\b Adds a new item in the list right before the last item.
 *	\p list Reference to the list that will be updated.
 *	\p item The element to insert into the list.
 *	\r 0 for success; -1 for error (only for invalid parameters).
 */
en_llist_ret_code llist_add_next(st_list *list, st_list_item *item);


/*************************************************************************************************/
/**
 *	\b Remove the item pointed by index from the list.
 *	\p list The list that the item will be removed.
 *	\p index The reference for the item.
 *	\r LLIST_RET_SUCCESS if the item was removed; LLIST_RET_NOTFOUND if the item pointed by index 
 *	was not found in the list; LLIST_RET_ERROR if received invalid parameters.
 */
en_llist_ret_code llist_rm_index(st_list *list, const unsigned int index);

/*************************************************************************************************/
/**
 *	\b Retrieve the first item from the list.
 *	\p mlist The list to retrieve the element from.
 *	\r Will return a pointer for the first item or NULL if there is not element in the list.
 */
st_list_item *llist_get_first(st_list *mlist);

/*************************************************************************************************/
/**
 *	\b Retrieve the last item from the list.
 *	\p mlist The list to retrieve the element from.
 *	\r Will return a pointer for the last item or NULL if there is not element in the list.
 */
st_list_item *llist_get_last(st_list *mlist);

/*************************************************************************************************/
/**
 *	\b Returns the item specified by index
 *	\p mlist The list to retrieve the element from.
 *	\p index The reference for the item inside the list.
 *	\n The way that the list is being iterated is very ugly. But
 *	this library should be used just for study purpose.
 */
st_list_item *llist_get_item(st_list *mlist, unsigned int index);

/*************************************************************************************************/
/**
 *	\b Allocate memory to the linked list.
 *	\r 0 for successfully operation; -1 if could not allocate memoryf for the list.
 */
en_llist_ret_code llist_init(st_list **mlist);

/*************************************************************************************************/
/**
 *	\b Free all list elements.
 *	\p list The list itself to be freed.
 */
void llist_free_items(st_list **list);

/*************************************************************************************************/
/**
 *	\b Destroys the list and all his members.
 *	\p list The list itself to be freed.
 *	\n  Should be used if you called llist_init() to initialize the list.
 */
void llist_destroy(st_list **list);

#endif /* LLIST_HEADER */
