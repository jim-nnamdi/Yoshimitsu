#include <stdio.h>
#include <stdlib.h>


struct node 
{
  int data;
  struct node* next;
};

/* create thread linked lists dynamically */
void create_thread_list()
{
  struct node *head = malloc(sizeof(struct node));
  head->data = 6; 
  head->next = NULL;

  /* second node with data next allocated */
  struct node *current = malloc(sizeof(struct node));
  current->data = 2;
  current->next = NULL;
  head->next = current;

  /* references another node - update link with current */
  current = malloc(sizeof(struct node));
  current->data = 4;
  current->next = NULL;
  head->next->next = current;
}

/* traverse the list */
void traverse_thread_list(struct node *head)
{
  if (head == NULL)
  {
    fprintf(stderr, "head is empty");
    exit(EXIT_FAILURE);
  }

  struct node *ptr = malloc(sizeof(struct node));
  ptr = head;
  while(ptr != NULL)
  {
    printf("%d\t", ptr->data);
    ptr = ptr->next;
  }
}

/* count number of thread nodes */
void thread_node_count(struct node *head)
{
  int count = 0;
  if(head == NULL)
  {
    fprintf(stderr, "head is empty");
    exit(EXIT_FAILURE);
  }
  struct node *ptr = malloc(sizeof(struct node));
  ptr = head;
  while(ptr != NULL)
  {
    count++;
    ptr = ptr->next;
  }
  printf("thread nodes: %d\n", count);
}

/* add a socket node at rear end of network server */
void socket_thread_rear_end(struct node *head, int rear_data)
{
  if (head == NULL)
  {
    fprintf(stderr, "head is empty");
    exit(EXIT_FAILURE);
  }
  /* allocate memory for both ptr & rear */
  struct node *ptr = malloc(sizeof(struct node));
  struct node *rear = malloc(sizeof(struct node));
  rear->data = rear_data;
  rear->next = NULL;
  ptr = head;
  while(ptr->next != NULL)
  {
    /* provided the link is null */
    /* move to the next link */
    ptr = ptr->next;
  }
  /* when you find where the next link is NULL */
  /* set the data to be rear ... */
  ptr->next = rear;
}

/* reverse the socket nodes data */
struct node * reverse_thread_nodes(struct node *head)
{
  if (head == NULL)
  {
    printf("nothing to reverse");
    exit(EXIT_SUCCESS);
  }

  struct node *prev = NULL;
  struct node *front = NULL;
  while(head != NULL)
  {
    /* perform link swap operations */
    front = head->next;
    head->next = prev;
    prev = head;
    head = front;
  }
  head = prev;
  return head;
}