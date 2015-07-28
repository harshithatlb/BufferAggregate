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
//	int offset;
};

typedef struct List Node;
// my buffer aggr ADT is a linked list of buffers
// ordered list can be done later

Node* head = NULL;
void split(Node*,char*,int, int);
CharBuffer* createBuffer(char* str,int size,int offset){
	printf("\n in create buffer");
	CharBuffer* charbuf = (CharBuffer*)malloc(sizeof(CharBuffer));
	(charbuf)->buf = (char*)malloc(sizeof(char)*size);
	strcpy((charbuf)->buf,str);
	(charbuf)->offset = offset;
	(charbuf)->size = size;
	return charbuf;
}

Node* createNode(){
	printf("\n creating node");
	Node* n = (Node*)malloc(sizeof(Node));
	n->next = NULL;
	return n;
}
/* Frees the memory allocated for buffer */
void freeCharData(CharBuffer* x){
	printf("\n In Free Char data");
	if(x!=NULL){
		free(x->buf);
	}
}

/* Frees the memory allocated for the Node*/
void freeNode(Node* x){
	printf("\n Freeing Node");
	if(x!=NULL)
	{
		if(x->data!=NULL){
			free(x->data->buf);
			free(x->data);
			free(x);
		}
	}
	printf("\n done with freeing");
}
/* allocates memory for head node*/
Node* createHead(){
	
	printf("\n create Node head");
	Node *head = (Node*)malloc(sizeof(Node));
	(head)->next = NULL;
	printf("\n returning from create Node head");
	return head;
}
/* inserts new buffer to end of list */
void insertToList(Node** head, char* str, int size,int offset){
	printf("\ninserting to list%s",str);
	if((*head)->data==NULL){
	//	*head = createHead();
		printf("\ncallin createBuffer");
		(*head)->data =(CharBuffer*)malloc(sizeof(CharBuffer));
		(*head)->data->offset = offset;
		(*head)->data->size = size;
		(*head)->data->buf = (char*)malloc(sizeof(char)*size);
		strcpy((*head)->data->buf,str); 
	//	printf("value in the buffer is%s",(head)->data->buf);
		printf("\nreturning from insert to list");
		return;
	}
//	Node* new_ptr;
	Node* new_ptr = (Node*)malloc(sizeof(Node));
	new_ptr->next = NULL;
	
	int offset_exists = findOffset(*head,offset,&new_ptr);
	
	printf("\n offset_exists:%d", offset_exists);
	
	if(offset_exists == 0){
		Node* x= *head;
		/* go to the last node*/
		while(x->next!=NULL)
			x=x->next;
		/* create a new Node*/
		Node* newNode = createNode();
	 	newNode->data=createBuffer(str, size,offset);
		/* make the last node point to the newly created node*/
		x->next = newNode;
		return;
	}
	else if(offset_exists==1){
		printf("\noffset exists");	
		printf("\n existing offset is %s",new_ptr->data->buf);
		split(new_ptr, str,offset,size);
	}
}

void split(Node*p, char* str,int offset, int size){
	int org_size = p->data->size;
	int org_offset = p->data->offset;	
	// freeing at the end
	char* org_str= (char*)malloc(sizeof(org_size));
	memcpy(org_str,p->data->buf,org_size);

	//Memory for new node holding offset to be inserted
	Node *q = (Node*)malloc(sizeof(Node));
	q->data = (CharBuffer*)malloc(sizeof(CharBuffer));
	int part_two_size = (p->data->offset+p->data->size)- offset;

	int size1=p->data->offset;
	int size2= p->data->size;
	
	printf("\n part two size :=%d",part_two_size);
	q->data->buf = (char*)malloc(sizeof(char)*part_two_size);

	printf("\nallocation done for size of part two");	
	char *buf_ptr = p->data->buf;

	char *rev = buf_ptr;
	rev = rev+offset;
	printf("\n printing the second half:%s",rev);	
	memcpy(q->data->buf,buf_ptr+offset,part_two_size*sizeof(char));

	printf("\n memcopied");
	q->data->size = size;
	q->data->offset = offset;
	q->next = p->next;
	p->next = q;
	
	//copy to p
	
	int prev_size = q->data->offset - p->data->offset;
	int prev_offset =p->data->offset;
	char* prev_data = (char*) malloc(sizeof(char)*prev_size);
	memcpy(prev_data,p->data->buf,sizeof(char)*prev_size);
	free(p->data->buf);
	p->data->buf = prev_data;
	p->data->offset = prev_offset;
	p->data->size = prev_size;
	
	printf("\n new pointer:%s",q->data->buf);
	if(q->data->offset+ q->data->size < org_offset+org_size){
		int r_size =(org_offset+org_size)-( q->data->offset+q->data->size);
		Node* r =(Node*)malloc(sizeof(Node));
		r->data = (CharBuffer*)malloc(sizeof(Node));
		r->data->buf = (char*)malloc(sizeof(char)*r_size);
		
		r->data->offset = q->data->offset+q->data->size;
		r->data->size = r_size;
		memcpy(r->data->buf,org_str+r->data->offset,sizeof(char)*size);
		
		r->next = q->next->next;
		q->next = r;
	}
	free(org->str);
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


void removeFromList(Node** head, int offset){
	//int location = findOffset(head, offset);
	printf("\n removing from list ");
	Node* p = *head;
	int i =0;
	if((*head)->data->offset == offset)
	{
		*head = (*head)->next;
		freeNode(p);
		return;
	}
	while(p->next!=NULL) {
		if(p->next->data->offset == offset)	
			break;
		p = p->next;
	}
	
	printf("\n printing value of prev node%d", p->data->offset);	
	
	Node* q = p->next;
	if(p->next!=NULL)
	p->next = p->next->next;
	freeNode(q);
	printf("\n removed node");
}

void printCharBuffer(CharBuffer *ptr){
	printf("\nPRINT:%d,%d",ptr->offset, ptr->size);
}
void printAggr(Node* aggr){
	Node* ptr = aggr;
	printf("\nin print aggr");
	printf("\n PRINTING:%d",ptr->data->offset);
	while(ptr!=NULL){
		printf("\n %d: %d: %s:", ptr->data->offset,ptr->data->size,ptr->data->buf);
		ptr = ptr->next;	
	}
}

void freeList(Node** head){
	printf("\nIn freeing List");
	Node* p = *head;
	while((*head)->next!=NULL){
		*head = (*head)->next;	
		freeNode(p);
		p = *head;
	}
}

main(){
	char *p = "harsh";
	head = (Node*)malloc(sizeof(Node)); 
	head->next = NULL;
	insertToList(&head,p, 5, 0);
	insertToList(&head,"abcdef",6,7);
	insertToList(&head,"harshitha",9,13);
	insertToList(&head,"hhh",3,1);
//	printAggr(head);
	//removeFromList(&head,7);
	printf("\n printing data");
	printAggr(head);
	freeList(&head);
}
