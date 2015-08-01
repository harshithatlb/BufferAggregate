#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* Buffer Aggregate Implementation
   The buffer aggregate container is a linked list of data buffers
*/
typedef struct{
	char* buf;
	int offset;
	int size;
}CharBuffer;

struct List{
	struct List* next;
	CharBuffer* data;
};

typedef struct List Node;
// my buffer aggr ADT is a linked list of buffers
// ordered list can be done later

Node* head = NULL;

void split(Node*,char*,int, int);
void freeNode(Node* x);
Node* createHead();
void insertToList(Node** x,char* str, int size, int offset);
int findOffset(Node* head,int offset,Node** new_ptr);
int removeFromList(Node** head, int offset);
void printAggr(Node* aggr);
void freeList(Node** head);

/* Frees the memory allocated for the Node*/
void freeNode(Node* x){
	if(x!=NULL)
	{
		if(x->data!=NULL){
			free(x->data->buf);
			free(x->data);
			free(x);
		}
	}
}
/* allocates memory for head node*/
Node* createHead(){
	
	Node *head = (Node*)malloc(sizeof(Node));
	(head)->next = NULL;
	return head;
}
/* inserts new buffer to end of list */
void insertToList(Node** head, char* str, int size,int offset){
	if((*head)->data == NULL){
		(*head)->data =(CharBuffer*)malloc(sizeof(CharBuffer));
		(*head)->data->offset = offset;
		(*head)->data->size = size;
		(*head)->data->buf = (char*)malloc(sizeof(char)*(size));
		strcpy((*head)->data->buf,str); 
		return;
	}
	Node* new_ptr= (Node*)malloc(sizeof(Node));
	new_ptr->next = NULL;
	
	int offset_exists = findOffset(*head,offset,&new_ptr);
	if(offset_exists == 0){
		Node* x= *head;
		/* go to the last node*/
		while(x->next!=NULL)
			x=x->next;
		/* create a new Node*/
		Node* newNode = (Node*)malloc(sizeof(Node));
		newNode->next = NULL;
	 	CharBuffer* charbuf = (CharBuffer*)malloc(sizeof(CharBuffer));
		(charbuf)->buf = (char*)malloc(sizeof(char)*(size));
		strcpy((charbuf)->buf,str);
		(charbuf)->offset = offset;
		(charbuf)->size = size;
		
		/* make the last node point to the newly created node*/
		newNode->data = charbuf;
		x->next = newNode;
		free(new_ptr);
		return;
	}

	else if(offset_exists == 1){
		split(new_ptr, str,offset,size);
		new_ptr = NULL;
		free(new_ptr);
	}
}

void split(Node*p, char* str,int offset, int size){
	// what is org_size
	int org_size = p->data->size;
	int org_offset = p->data->offset;	
	//  creating a copy of the buffer's data
	char* org_str = (char*)malloc(sizeof(org_size));
	memcpy(org_str,p->data->buf,org_size);

	//Memory for new node holding offset to be inserted
	Node *q = (Node*)malloc(sizeof(Node));
	q->data = (CharBuffer*)malloc(sizeof(CharBuffer));

	int part_two_size = (p->data->offset+p->data->size)- offset;
	int size1 = p->data->offset;
	int size2 = p->data->size;
	
	q->data->buf = (char*)malloc(sizeof(char)*part_two_size);

	char *buf_ptr = p->data->buf;

	char *rev = buf_ptr;
	rev = rev+offset;
	//copying the content of data after the offset at split to q
	memcpy(q->data->buf,buf_ptr+offset,part_two_size*sizeof(char));

	q->data->size = size;
	q->data->offset = offset;
	q->next = p->next;
	p->next = q;
	
	//copy to p
	
	int prev_size = q->data->offset - p->data->offset;
	int prev_offset = p->data->offset;
	char* prev_data = (char*) malloc(sizeof(char)*prev_size);
	memcpy(prev_data,p->data->buf,sizeof(char)*prev_size);
	// freeing the data buffer 
	free(p->data->buf);
	p->data->buf = prev_data;
	p->data->offset = prev_offset;
	p->data->size = prev_size;
	
	if(q->data->offset+ q->data->size < org_offset+org_size){
		int r_size = q->data->size - size+1;
		int qsize = q->data->size;
		q->data->size = size;
		free(q->data->buf);
		q->data->buf = (char*)malloc(sizeof(char)*size);
		memcpy(q->data->buf,str,sizeof(char)*size);

		Node* r =(Node*)malloc(sizeof(Node));
		r->data = (CharBuffer*)malloc(sizeof(Node));
		r->data->buf = (char*)malloc(sizeof(char)*r_size);
		r->data->offset = q->data->offset+size;
		r->data->size = r_size;
		memcpy(r->data->buf,org_str+r->data->offset,sizeof(char)*r_size);
		r->next = q->next;
		q->next = r;
	}
	free(org_str);
	
}

/* returns the position if the offset already exists 
	returns 1 when offset is found and sets the pointer to the
	existing buffer
*/
int findOffset(Node* head,int offset,Node** new_ptr){
	Node* p = head;
	int i =0;
	while(p!=NULL){
		if((p->data->offset+ p->data->size) > offset){
			*new_ptr = p;
			return 1;
		}
		p=p->next;
	}
	return 0;
}
/*  removes the buffer from buffer aggr linked list 
	if the offset matched 
*/
int removeFromList(Node** head, int offset){
	//int location = findOffset(head, offset);
	Node* p = *head;
	int retval= 0;
	int i =0;
	// if the node to be removed is head
	if((*head)->data->offset == offset)
	{
		*head = (*head)->next;
		freeNode(p);
		return 1;
	}
	// move to the node before p, p is the one to be removed
	while(p->next!=NULL) {
		if(p->next->data->offset == offset)	{
			retval = 1;
			break;
		}
		p = p->next;
	}
	// q is the node to be removed
	Node* q;
	q = p->next;
	
	if(p->next->next!=NULL)
			p->next = p->next->next;
	freeNode(q);
	return retval;
}

/* to print the contents of the ADT */
void printAggr(Node* aggr){
	Node* ptr = aggr;
	while(ptr!=NULL){
		printf("\n %d: %d: %s", ptr->data->offset,ptr->data->size,ptr->data->buf);
		ptr = ptr->next;	
	}
}

/* 	Free the ADT*/
void freeList(Node** head){
	Node* p = *head;
	while(p->next!=NULL){
		printf("\n%s",p->data->buf);
		freeNode(p);
		p= p->next;
	}
}

void read(int start, int end){
	Node* q = head;
	CharBuffer* cb;
	char str[end-start+1];
	int index = 0;
	while(q!=NULL && start<= end){
		cb = q->data;
		if( (start >= cb->offset) && (end < (cb->offset + cb->size)) ){
			strncpy(str+index,cb->buf+(start-cb->offset),(end - start+1));
			break;
		}
		else if((cb->offset<= start) &&(cb->offset+cb->size)>start){
			strncpy(str+index,cb->buf+(start-cb->offset), (cb->offset + cb->size-start));
			index+= ( cb->offset + cb->size -  start );
			if(q->next != NULL)
				start = q->next->data->offset;
		}
		q = q->next;
	}
}
main(){
	char *p = "harsh";
	head = (Node*)malloc(sizeof(Node)); 
	head->next = NULL;
	insertToList(&head,p, 5, 0);
//	insertToList(&head,"abcdef",6,7);
//	insertToList(&head,"harshitha",9,13);
//	insertToList(&head,"hhh",3,1);
//	printAggr(head);
	//removeFromList(&head,7);
	printAggr(head);
//	read(7,15);
	freeList(&head);
}
