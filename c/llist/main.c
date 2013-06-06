/*
 * brief: main program used to develop and test the linked_list.
 * author: vitor.rozsa
 * date: 14/02/2013
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "llist.h"


typedef struct {
	char name[32];
	char *l;
	int counter;
} myStruct;

st_list *test(void);
static void feed_list(int qnt, st_list *list);
static void list_dump_item(st_list_item *listItem);
static void list_dump(st_list *mlist);


int main(int argc, char *argv[])
{
	st_list *myList = NULL;
	st_list_item *item = NULL;
	en_llist_ret_code ret;

	myList = test();

	if (!myList) {
		fprintf(stderr, "Failed to operate with linke_list!\n");
		return -1;
	}

	// prints all elements from the list
	list_dump(myList);

	// prints the first item from the list
	item = llist_get_first(myList);
	if (!item) {
		fprintf(stderr, "Failed to retrieve an item from the list");
		llist_destroy(&myList);
		return -1;
	}
	list_dump_item(item);

	myStruct *mst = (myStruct *) item->data;
	free(mst->l);

	// prints the last item from the list
	item = llist_get_last(myList);
	if (!item) {
		fprintf(stderr, "Failed to retrieve an item from the list");
		llist_destroy(&myList);
		return -1;
	}
	list_dump_item(item);

	// get an item at the middle of the list
	item = llist_get_item(myList, 4);
	if (!item) {
		fprintf(stderr, "Failed to retrieve an item from the list");
		llist_destroy(&myList);
		return -1;
	}
	list_dump_item(item);

	// Remove test.
	printf("\nWill remove some items of the list!!\n");
	// First.
	ret = llist_rm_index(myList, 0);
	if (ret != LLIST_RET_SUCCESS) {
		fprintf(stderr, "Failed to remove first item.\n");
	}

	// Middle.
	ret = llist_rm_index(myList, 5);
	if (ret != LLIST_RET_SUCCESS) {
		fprintf(stderr, "Failed to remove middle item.\n");
	}

	// last.
	item = llist_get_last(myList);
	if (!item) {
		fprintf(stderr, "Failed to retrieve an item from the list");
		llist_destroy(&myList);
		return -1;
	}
	mst = (myStruct *) item->data;
	free(mst->l);

	ret = llist_rm_index(myList, item->index);
	if (ret != LLIST_RET_SUCCESS) {
		fprintf(stderr, "Failed to remove last item.\n");
	}

	list_dump(myList);

	// try to remove an invalid item.
	ret = llist_rm_index(myList, 9);
	if (ret == LLIST_RET_NOTFOUND) {
		fprintf(stderr, "[expected] Failed to remove last item. Item not found!\n");
	}
	else if (ret != LLIST_RET_SUCCESS) {
		fprintf(stderr, "Something went wrong while trying to remove last item.\n");
	}

	// free the list elements and erase the reference
	llist_destroy(&myList);

	return 0;
}

/*
 * brief Initializes and fill up a new linked_list
 * return The created linked_list
 */
st_list *test(void)
{
	st_list *myList = NULL;
	st_list_item *myFirstItem = NULL;
	myStruct *data = NULL;

	myList = (st_list *)malloc(sizeof(st_list));
	if (!myList) {
		fprintf(stderr, "test() - malloc for myList failed!!\n");
		return NULL;
	}

	myFirstItem = (st_list_item *)malloc(sizeof(st_list_item));
	if (!myFirstItem) {
		llist_destroy(&myList);
		fprintf(stderr, "test() - malloc for myFirstItem failed!!\n");
		return NULL;
	}

	data = (myStruct *)malloc(sizeof(myStruct));
	if (!data) {
		llist_destroy(&myList);
		free(myFirstItem);
		fprintf(stderr, "test() - malloc for data failed!!\n");
		return NULL;
	}

	memset(myFirstItem, 0, sizeof(st_list_item));
	memset(data, 0 , sizeof(myStruct));

	strncpy(data->name, "first item!", sizeof(data->name));
	data->counter = 0;
	data->l = (char *)malloc(sizeof(char) * 5);
	if (data->l == NULL) {
		printf("Failed to malloc data to data->l.\n");
	}

	myFirstItem->data = data;

	llist_add_first(myList, myFirstItem);

	// fill the list
	feed_list(10, myList);

	return myList;
}

/*
 * brief Fill up the list with default incremental values
 */
static void feed_list(int qnt, st_list *list)
{
	int i;
	char buf[32];
	st_list_item *listItem = NULL;
	myStruct *data = NULL;

	if (!list) {
		return;
	}

	for (i = 1; i < qnt; i++) {

		listItem = (st_list_item *) malloc(sizeof(st_list_item));
		if (!listItem) {
			fprintf(stderr, "feed_list - Failed to alloc memory for st_list_item!!\n");
			return;
		}

		data = (myStruct *) malloc(sizeof(myStruct));
		if (!data) {
			fprintf(stderr, "feed_list - Failed to alloc memory for data!!\n");
			free(listItem);
			return;
		}

		memset(listItem, 0, sizeof(st_list_item));
		memset(data, 0, sizeof(myStruct));

		snprintf(data->name, sizeof(data->name),"item number %d", i);
		data->counter = i*10;

		listItem->data = data;

		llist_add_next(list, listItem);

		listItem = NULL;
	}
}

/*
 * brief Prints all members in the list
 */
static void list_dump(st_list *mlist)
{
	st_list_item *next = NULL;

	if ((mlist == NULL) || (mlist->first == NULL)) {
		return;
	}
	next = mlist->first->next;
	list_dump_item(mlist->first);


	while(next) {
		list_dump_item(next);
		next = next->next;
	}
}

/*
 * brief Prints the given item
 */
static void list_dump_item(st_list_item *listItem)
{
	myStruct *data = NULL;

	if (listItem->data != NULL) {
		data = (myStruct *)listItem->data;
		printf("\ncounter: %d\nname: %s\n\n", data->counter, data->name);
	}
}

