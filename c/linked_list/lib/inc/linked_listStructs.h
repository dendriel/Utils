/*
 * brief: linked list library example - structures header
 * author: vitor.rozsa
 * date: 14/02/2013
 */
#ifndef LINKED_LIST_STRUCTS_H_INCLUDED
#define LINKED_LIST_STRUCTS_H_INCLUDED

typedef struct {
	void *data;
	int index;
	void *next;
} st_list_item;

typedef struct {
	st_list_item *first;
	st_list_item *last;
	int item_counter;
} st_list;

#endif /* LINKED_LIST_STRUCTS_H_INCLUDED */
