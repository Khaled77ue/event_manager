#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


typedef struct node{

    PQElement element;
    PQElementPriority priority;
    int counter_node;
    struct node *next;

}*Node;

struct PriorityQueue_t{
    Node head;
    Node it;
    int size;
    int counter;
    PQElement  iterator;
    CopyPQElement copy;
    CopyPQElementPriority copy_P;
    FreePQElement free;
    FreePQElementPriority free_P;
    EqualPQElements equal;
    ComparePQElementPriorities  cmp;
};

/**
* createNode1: Allocates a new node.
*
* @param queue - the priority queue that we want to make a copy of.
* @param element - the element that we want to put in node.
* @param priority - the priority of the elemnt that we want to put in node.
* @param counter - the counter in the priority queue that we want to put in node.
* @return
* NULL - if allocation failed.
* otherwise a new node.
*/
static Node createNode1(PriorityQueue queue,PQElement element,PQElementPriority priority,int counter)
{
    Node ptr = malloc(sizeof(*ptr));
    if(!ptr) {
        return NULL;
    }
    assert(ptr!=NULL);
    ptr->element=queue->copy(element);
    ptr->priority=queue->copy_P(priority);
    ptr->counter_node=counter;
    return ptr;
}

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities)
{
    if(copy_element==NULL||free_element==NULL||equal_elements==NULL||copy_priority==NULL||free_priority==NULL
    ||compare_priorities==NULL)
    {
        return NULL;
    }
    PriorityQueue queue= malloc(sizeof(*queue));
    Node first=malloc(sizeof(*first));
    queue->head= first;
    first->next=NULL;
    first->element=NULL;
    first->priority=NULL;
    queue-> iterator=NULL;
    queue->size=0;
    queue->counter=0;
    queue->copy=copy_element;
    queue->copy_P=copy_priority;
    queue->free=free_element;
    queue->free_P=free_priority;
    queue->cmp=compare_priorities;
    queue->equal=equal_elements;
    return queue;
}
void pqDestroy(PriorityQueue queue)
{
    if(queue==NULL){
        return;
    }
    Node current=queue->head;
    while (current!=NULL)
    {
        if(current->element)
        {
            queue->free(current->element);
        }
        if(current->priority)
        {
            queue->free_P(current->priority);
        }
        Node s=current;
        current=current->next;
        free(s);
    }
    if(queue->iterator!=NULL)
    {
        queue->free(queue->iterator);
    }
    free(queue);
}

PriorityQueue pqCopy(PriorityQueue queue)
{
    if(queue==NULL)
    {
        return NULL;
    }
    PriorityQueue  newqueue= pqCreate(queue->copy,
                                      queue->free,
                                      queue->equal,
                                      queue->copy_P,
                                      queue->free_P,
                                      queue->cmp);
    Node current_node= queue->head;
    while(current_node!=NULL)
    {
        pqInsert(newqueue,current_node->element,current_node->priority);
        current_node=current_node->next;
    }
    queue->it=NULL;
    newqueue->it=NULL;
    newqueue->size=queue->size;
    newqueue->counter=queue->counter;
    return newqueue;
}

int pqGetSize(PriorityQueue queue)
{
    if(!queue)
    {
        return -1;
    }
    return queue->size;
}

bool pqContains(PriorityQueue queue, PQElement element)
{
    if(!element)
    {return false;}
    Node current_node= queue->head;
    while(current_node!=NULL){
        if(queue->equal(current_node->element,element))
            return true;
        current_node=current_node->next;
    }
    return false;
}
PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority)
{
    if(queue==NULL||element==NULL||priority==NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    if(queue->size==0)
    {
        queue->head->element=queue->copy(element);
        queue->head->priority=queue->copy_P(priority);
        queue->head->counter_node=queue->counter;
        queue->size++;
        queue->counter++;
        queue->it=NULL;
        return PQ_SUCCESS;
    }
    Node current=queue->head;
    if(queue->cmp(priority,current->priority)>0){
        queue->head=createNode1(queue,element,priority,queue->counter);
        if(queue->head==NULL)
        {
            return PQ_OUT_OF_MEMORY;
        }
        queue->head->next=current;
        queue->size++;
        queue->counter++;
        queue->it=NULL;
        return PQ_SUCCESS;
    }
    Node new=createNode1(queue,element,priority,queue->counter);
    if(new==NULL)
        return PQ_OUT_OF_MEMORY;
    Node prev=current;
    current=current->next;
    while(current!=NULL){
        if(queue->cmp(priority,prev->priority)<=0&&queue->cmp(priority,current->priority)>0){
            prev->next=new;
            new->next=current;
            queue->size++;
            queue->counter++;
            queue->it=NULL;
            return  PQ_SUCCESS;
        }
        prev=prev->next;
        current=current->next;
    }
    if(current==NULL){
        prev->next=new;
        new->next=NULL;
    }
    queue->size++;
    queue->counter++;
    queue->it=NULL;
    return PQ_SUCCESS;
}


/**
* pqRemoveElement_priority: removes the priority of the element.
*
* @param queue - the priority queue that we want to remove from.
* @param element - the element that we want to delete its priority.
* @param priority - the priority of the element we are looking for to delete.
*/
static void pqRemoveElement_priorty(PriorityQueue queue, PQElement element,
                             PQElementPriority priority)
{
    int counter=0;
    Node current=queue->head;
    Node prev=current;
    Node prev_to_remove=current;
    if(queue->equal(queue->head->element,element)&&queue->cmp(queue->head->priority,
            priority)==0&&queue->size>=2)
    {
        Node cur=queue->head;
        queue->head=queue->head->next;
        queue->free(cur->element);
        queue->free_P(cur->priority);
        free(cur);
        return;
    }
    if(current->next==NULL)
    {
        queue->free(current->element);
        queue->free_P(current->priority);
        current->element=NULL;
        current->priority=NULL;
        return;
    }
    while(current!=NULL)
    {
        if (queue->equal(element,current->element)&&queue->cmp(priority,current->priority)==0)
        {
            break;
        }
        prev=current;
        current=current->next;
    }
    if(!current)
    {
        return;
    }
    counter=current->counter_node;
    Node node_to_remove=current;
    prev_to_remove=prev;
    prev=current;
    current=current->next;
    while(current!=NULL&&queue->equal(element,current->element)&&queue->cmp(priority,current)==0)
    {
        if(current->counter_node<prev->counter_node&&current->counter_node<counter)
        {
            node_to_remove=current;
            prev_to_remove=prev;
            counter=current->counter_node;
        }
        current=current->next;
        prev=prev->next;
    }
    if(prev_to_remove==node_to_remove&&node_to_remove==queue->head)
    {
        queue->head=node_to_remove->next;
        queue->free(node_to_remove->element);
        queue->free_P(node_to_remove->priority);
        free(node_to_remove);
        return;
    }
    if(prev_to_remove!=NULL&&node_to_remove!=NULL)
    {
        prev_to_remove->next=node_to_remove->next;
        queue->free(node_to_remove->element);
        queue->free_P(node_to_remove->priority);
        free(node_to_remove);
    }
}

/**
* pqContains_P: check if the priority queue contains a priority.
*
* @param queue - the priority queue that we want to search in.
* @param element - the element that we want to find its priority.
* @param priority - the priority of the element we are looking for.
* @return
* returns true if the queue contains the priority.
* otherwise false.
*/
static bool pqContains_P(PriorityQueue queue, PQElement element,
                  PQElementPriority priority)
{
    if(!element)
    {
        return false;
    }
    Node current_node= queue->head;
    while(current_node!=NULL){
        if(queue->equal(current_node->element,element)&&queue->cmp(priority,current_node->priority)==0)
        {
            return true;
        }
        current_node=current_node->next;
    }
    return false;
}

PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority)
{
    if(queue==NULL||element==NULL||old_priority==NULL||new_priority==NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    if(!pqContains_P(queue, element, old_priority))
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    if(queue->size==1)
    {
        queue->free_P(queue->head->priority);
        queue->head->priority= queue->copy_P(new_priority);
        return PQ_SUCCESS;
    }
    PQElement new_element=queue->copy(element);
    pqRemoveElement_priorty(queue, element,old_priority);
    pqInsert(queue,new_element,new_priority);
    queue->free(new_element);
    queue->it=NULL;
    return PQ_SUCCESS;
}

PriorityQueueResult pqRemove(PriorityQueue queue)
{
    if(queue==NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node current=queue->head;
    queue->head=current->next;
    queue->free(current->element);
    queue->free_P(current->priority);
    free(current);
    queue->size--;
    queue->it=NULL;
    return PQ_SUCCESS;
}


PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element)
{
    if(queue==NULL||element==NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node current=queue->head;
    if(!pqContains(queue, element))
    {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }
    if(queue->equal(current->element,element))
    {
        pqRemoveElement_priorty(queue,element,current->priority);
        queue->size--;
        queue->it=NULL;
        return PQ_SUCCESS;
    }
    current=current->next;
    while (current!=NULL)
    {
        if(queue->equal(current->element,element))
        {
            pqRemoveElement_priorty(queue,element,current->priority);
            queue->size--;
            queue->it=NULL;
            return PQ_SUCCESS;
        }
        current=current->next;
    }
    queue->it=NULL;
    return PQ_ERROR;
}


PQElement pqGetFirst(PriorityQueue queue)
{
    if(!queue->head)
    {
        return NULL;
    }
    if(!queue->head->element)
    {
        return NULL;
    }
    queue->it=queue->head;
    if(queue->iterator)
    {
        queue->free(queue->iterator);
    }
    queue->iterator=queue->copy(queue->head->element);
    return queue->iterator;
}


PQElement pqGetNext(PriorityQueue queue)
{
    if(queue->it==NULL||queue->it->next==NULL)
    {
        return NULL;
    }
    queue->it=queue->it->next;
    if(queue->iterator) {
        queue->free(queue->iterator);
    }
    queue->iterator=queue->copy(queue->it->element);
    return queue->iterator;
}


PriorityQueueResult pqClear(PriorityQueue queue)
{
    if(queue==NULL)
    {
        return PQ_NULL_ARGUMENT;
    }
    Node current=queue->head;
    queue->free(current->element);
    queue->free_P(current->priority);
    current->element=NULL;
    current->priority=NULL;
    current=current->next;
    while(current!=NULL)
    {
        queue->free(current->element);
        queue->free_P(current->priority);
        Node p1=current;
        current=current->next;
        free(p1);
    }
    queue->head->next=NULL;
    queue->iterator=NULL;
    queue->it=queue->head;
    queue->counter=0;
    queue->size=0;
    return PQ_SUCCESS;
}






