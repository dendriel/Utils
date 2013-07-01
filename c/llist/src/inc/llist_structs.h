/***************************************************************************************************
 * Description: linked list library example - structures header.
 * Author: vitor.rozsa
 * Contact: vitor.rozsa@hotmail.com
 * Creation date: 14/02/2013
 **************************************************************************************************/
#ifndef LINKED_LIST_STRUCTS_HEADER
#define LINKED_LIST_STRUCTS_HEADER

/**
 *	\b Generic item structure.
 */
typedef struct st_list_item {
	void *data;
	unsigned int index;
	struct st_list_item *next;
	struct st_list_item *prev;
} st_list_item;

/**
 *	\b Generic list structure.
 */
typedef struct {
	st_list_item *first;
	st_list_item *last;
	unsigned int item_counter;
	unsigned int item_index;
} st_list;

#endif /* LINKED_LIST_STRUCTS_HEADER */
