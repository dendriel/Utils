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
typedef struct {
	void *data;
	unsigned int index;
	void *next;
	void *prev;
} st_list_item;

/**
 *	\b Generic list structure.
 */
typedef struct {
	st_list_item *first;
	st_list_item *last;
	unsigned int item_counter;
} st_list;

#endif /* LINKED_LIST_STRUCTS_HEADER */
