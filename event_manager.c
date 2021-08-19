#include "date.h"
#include "event_manager.h"
#include "priority_queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#define INVALID_MONTH 0
#define MIN_DAY 1
#define MAX_DAY 30
#define MONTHS_NUM 12
#define DAYS_IN_YEAR 365
#define MONTH_STR_LEN 4
#define NEGATIVE -1
#define POSITIVE 1

typedef struct node
{

    char *name;
    int id;
    int counter;
    Date date;
    struct node *next;

}*Node;

typedef struct event_element
{
    char *name;
    int id;
    Node members_head;
    int counter;
}*Event_element;

/**
* create_in_Node: creates a date in new node.
*
* @param date - the date to put in the node.

* @return
* NULL - if allocation fails.
* otherwise a new node.
*/
static Node create_in_Node(Date date)
{
    Node ptr = malloc(sizeof(*ptr));
    if(ptr == NULL) {
        return NULL;
    }
    assert(ptr!=NULL);
    ptr->name=NULL;
    ptr->id=0;
    ptr->counter=0;
    ptr->date=dateCopy(date);
    return ptr;
}

/**
* createNode: creates a new node.
*
* @param name - name to put in the node.
* @param id - the id to put in node.
* @param counter - counter  for the node.
* @param date - the date to put in node.
* @return
* NULL - if allocation fails.
* otherwise a new node.
*/

static Node createNode(char *name,int id,int counter,Date date)
{
    Node ptr = malloc(sizeof(*ptr));
    if(ptr == NULL) {
        return NULL;
    }
    assert(ptr!=NULL);
    if(name){
        char * new_name=malloc(sizeof(char)*strlen(name)+1);

        strcpy( new_name,name);
        ptr->name=new_name;
    }
    if(!name){
        ptr->name=name;
    }
    ptr->id=id;
    ptr->counter=counter;
    ptr->date=dateCopy(date);
    ptr->next=NULL;
    return ptr;
}


/**
* eventElement_create: creates a new event element.
*
* @param name - name of the element.
* @param id - the id of the element.
* @param counter - counter of elements.
* @return
* NULL - if allocation fails.
* otherwise a new event element.
*/
static Event_element eventElement_create(char* name,int id,int counter)
{
    Event_element element=malloc(sizeof(*element));
    char* new_name=malloc(sizeof(char)*strlen(name)+1);
    strcpy(new_name,name);
    element->name=new_name;
    element->id=id;
    element->counter=counter;
    element->members_head=NULL;
    return element;
}


struct EventManager_t
{
    PriorityQueue queue;
    Node head_events;
    int counter_num_of_events;
    Date begginig_date;
    Node members_in_sysem_head;
    int counter;
    int current_event_id;
};

/**
* Destroy_Node: destroys a given node.
*
* @param node - the node to destroy.
*/
static void Destroy_Node(Node node)
{
    if(node==NULL){
        return;
    }

    if(node->name==NULL&&node->date!=NULL)
    {
        dateDestroy(node->date);
        free(node);
        return;
    }

    Node current=node;

    if(current->next==NULL){
        free(node->name);
        dateDestroy(node->date);
        free(node);
        return;

    }
    while (current!=NULL){
        Node fr=current;
        current=current->next;
        free(fr->name);
        dateDestroy(fr->date);
        free(fr);
        fr=NULL;

    }


}

/**
* date_cmp: compare between two given dates.
*
* @param date1 - the first date.
* @param date2 - the second date.
* @return
* 		A positive integer if date1 occurs first;
* 		0 if they're equal;
*		A negative integer if date1 arrives after date2.
*/
static int date_cmp(Date date1, Date date2)
{
    return -1*dateCompare(date1,date2);
}

/**
* copy_Node: create a new copy of node.
*
* @param node - the node we want to copy.
* @return
* NULL - if allocation fails.
* otherwise a new copy of node.
*/
static Node copy_Node(Node node)
{
    if(!node)
    {
        return NULL;
    }
    Node new=create_in_Node(node->date);
    char*   new_name=malloc(sizeof(char)*strlen(node->name)+1);
    strcpy(new_name,node->name);
    new->name=new_name;
    new->id=node->id;
    new->counter=node->counter;
    new->next=NULL;
    Node current=node->next;
    Node current_new=new;
    while (current!=NULL){
        current_new->next=createNode(current->name,current->id,current->counter,current->date);
        current=current->next;
        current_new=current_new->next;
        if(current==NULL){
            current_new->next=NULL;
        }

    }

    return new;
}



/**
* copy_element: creates a new copy of element.
*
* @param element - the element to copy.
* @return
* NULL - if element is null.
* otherwise a new copy of element.
*/
static Event_element copy_element(Event_element element)
{
    if (!element)
    {
        return NULL;
    }
    Event_element new_element=eventElement_create(element->name,element->id,element->counter);
    new_element->members_head=copy_Node(element->members_head);
    return new_element;
}

/**
* free_element: free a given element.
*
* @param element - element to free.
*/
static void free_element(Event_element element)
{
    free(element->name);
    Destroy_Node(element->members_head);
    free(element);
}

/**
* equal_element: check if two elements are equal.
*
* @param element1 - first element.
* @param element2 - second element.
* @return
* FALSE - if one of the elements is NULL or not equal.
* TRUE - if they are equal.
*/
static bool equal_element(Event_element element1,Event_element element2)
{
    if((element1&&!element2)||(!element1&&element2)){
        return false;
    }
    if(strcmp(element1->name,element2->name)==0&&element1->id==element2->id)
    {
        return true;
    }
    else {
        return false;
    }
}







EventManager createEventManager(Date date)
{

    if(!date){
        return NULL;
    }
    Date   date2=dateCopy(date);
    if(date2==NULL)
    {
        return NULL;
    }
    dateDestroy(date2);
    EventManager eventManager=malloc(sizeof(*eventManager));
    eventManager->queue=pqCreate((CopyPQElement) copy_element, (FreePQElement) free_element,
                                 (EqualPQElements) equal_element, (CopyPQElementPriority) dateCopy,
                                 (FreePQElementPriority) dateDestroy,
                                 (ComparePQElementPriorities) date_cmp);
    // char* a=NULL;
    eventManager->head_events=create_in_Node(date);
    eventManager->head_events->next=NULL;
    eventManager->counter_num_of_events=0;
    eventManager->begginig_date=dateCopy(date);
    eventManager->members_in_sysem_head=create_in_Node(date);
    eventManager->members_in_sysem_head->next=NULL;
    eventManager->counter=0;
    eventManager->current_event_id=-1;
    return eventManager;
}

void destroyEventManager(EventManager em)
{
    if(em == NULL)
    {
        return;
    }
    pqDestroy(em->queue);
    Destroy_Node(em->head_events);
    dateDestroy(em->begginig_date);
    Destroy_Node(em->members_in_sysem_head);
    free(em);
}

/**
* there_is_event_the_same: check if there are two same events.
*
* @param head - the head to check in.
* @param date - the date of the events.
*  @param id - the id of the event.
* @return
* FALSE - if there is no two same events.
* otherwise TRUE.
*/
static bool there_is_event_the_same(Node head,int id,Date date)
{

    Node current=head;
    while(current!=NULL){
        if(current->id==id){
            break;
        }
        current=current->next;
    }
    if(!current)
    {
        return false;
    }

    Node current2=head;
    while(current2!=NULL){
        if(strcmp(current->name,current2->name)==0&&date_cmp(date,current2->date)==0){
            return true;
        }
        current2=current2->next;
    }
    return false;
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id)
{
    if(em==NULL||event_name==NULL)
    {
        return EM_NULL_ARGUMENT;
    }

    if(date==NULL)
    {
        return EM_INVALID_DATE;
    }
    if(date_cmp(em->begginig_date,date)<0){
        return EM_INVALID_DATE;
    }
    if(event_id<0)
    {
        return EM_INVALID_EVENT_ID;
    }


    if(em->counter_num_of_events==0){


        Event_element element=eventElement_create(event_name, event_id, em->counter);
        pqInsert(em->queue,element,date);
        free_element(element);

        em->head_events->id=event_id;
        char * new_name=malloc(sizeof(char)*strlen(event_name)+1);
        strcpy( new_name,event_name);
        em->head_events->counter=em->counter;
        em->head_events->name=new_name;
        if(em->head_events->date){
            dateDestroy(em->head_events->date);
        }
        em->head_events->date=dateCopy(date);
        em->counter++;
        em->counter_num_of_events++;
        return EM_SUCCESS;
    }

    Node currrent=em->head_events;
    while(currrent!=NULL){
        if(strcmp(currrent->name,event_name)==0&&date_cmp(currrent->date,date)==0)
        {
            return EM_EVENT_ALREADY_EXISTS;
        }
        if(currrent->id==event_id){
            return EM_EVENT_ID_ALREADY_EXISTS;
        }
        currrent=currrent->next;
    }




    Event_element element=eventElement_create(event_name, event_id, em->counter);
    pqInsert(em->queue,element,date);
    free_element(element);
    Node new=createNode(event_name,event_id,em->counter,date);
    new->next=em->head_events;
    em->head_events=new;

    em->counter++;
    em->counter_num_of_events++;
    return EM_SUCCESS;
}



EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id)
{
    if(em == NULL || event_name == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(days < 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }

    Date date_wanted1=dateCopy(em->begginig_date);
    if(em->counter_num_of_events>0){
        for(int i=0;i<days;i++)
            dateTick(date_wanted1);
        Node currrent1=em->head_events;
        while(currrent1!=NULL){
            if(strcmp(currrent1->name,event_name)==0&&dateCompare(currrent1->date,date_wanted1)==0){
                dateDestroy(date_wanted1);
                return EM_EVENT_ALREADY_EXISTS;
            }

            if(currrent1->id==event_id){
                dateDestroy(date_wanted1);
                return EM_EVENT_ID_ALREADY_EXISTS;
            }
            currrent1=currrent1->next;
        }
    }
    dateDestroy(date_wanted1);


    Node current=em->head_events;
    if(current->name==NULL&&current->id==0&&current->counter==0){
        Date date_wanted=dateCopy(em->begginig_date);

        for(int i=0;i<days;i++)
            dateTick(date_wanted);
        char * new_name=malloc(sizeof(char)*strlen(event_name)+1);
        strcpy(new_name,event_name);
        current->name=new_name;
        current->id = event_id;
        current->counter=em->counter;
        dateDestroy(current->date);
        current->date=dateCopy(date_wanted);
        current->next=NULL;
        Event_element event = eventElement_create(current->name,current->id,em->counter);
        pqInsert(em->queue,event,date_wanted);
        free_element(event);
        dateDestroy(date_wanted);
        em->counter_num_of_events +=1;
        em->counter+=1;
        return EM_SUCCESS;
    }

    while(current!=NULL)
    {

        if(current->id==event_id)
        {
            return EM_EVENT_ID_ALREADY_EXISTS;
        }
        current=current->next;
    }
    Date date_wanted=dateCopy(em->begginig_date);

    for(int i=0;i<days;i++)
        dateTick(date_wanted);




    Event_element event = eventElement_create(event_name,event_id,em->counter);
    pqInsert(em->queue,event,date_wanted);
    em->counter_num_of_events +=1;

    Node node_new=createNode(event_name,event_id,em->counter,date_wanted);
    node_new->next=em->head_events;
    em->head_events=node_new;
    em->counter+=1;
    dateDestroy(date_wanted);
    free_element(event);
    return EM_SUCCESS;
}



/**
* dec_one_from_member: remove one member.
*
* @param em - event manager we want to remove from.
* @param current_members - members in the event.
*/

static void  dec_one_from_member(EventManager em,Node current_members)
{
    Node cur=em->members_in_sysem_head;
    while(cur!=NULL){
        if(strcmp(cur->name,current_members->name)==0&&cur->id==current_members->id){
            cur->counter--;
            return;
        }
        cur=cur->next;
    }


}

/**
* Get_element: search for a specific element.
*
* @param queue - queue to search in.
* @param element - element we are looking for.
* @return
* returns the element we are looking for.
*/
static Event_element Get_element(PriorityQueue queue,Event_element element){
    Event_element wanted=(Event_element)pqGetFirst(queue);
    if(equal_element(element,wanted)){
        return copy_element(wanted);
    }
    wanted=pqGetNext(queue);
    while (wanted!=NULL){
        if(equal_element(element,wanted)){
            return copy_element(wanted);
        }
        wanted=pqGetNext(queue);
    }
    return wanted;

}

EventManagerResult emRemoveEvent(EventManager em, int event_id)
{

    if(em==NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id<0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Node current=em->head_events;
    if(em->counter_num_of_events==0){
        return EM_EVENT_NOT_EXISTS;
    }
    if(current->id==event_id){
        Event_element element=eventElement_create(current->name,current->id,current->counter);
        Event_element current1=Get_element(em->queue,element);
        pqRemoveElement(em->queue,element);
        em->head_events=em->head_events->next;
        free(current->name);
        em->counter_num_of_events--;
        Node current_members=current1->members_head;
        while(current_members!=NULL){
            dec_one_from_member(em,current_members);
            current_members=current_members->next;
        }
        free_element(current1);
        dateDestroy(current->date);
        free_element(element);
        free(current);
        return EM_SUCCESS;

    }
    Node prev=current;
    current=current->next;
    while(current!=NULL){
        if (current->id==event_id){
            Event_element element=eventElement_create(current->name,current->id,current->counter);
            Event_element current1=Get_element(em->queue,element);
            pqRemoveElement(em->queue,element);

            Node current_members=current1->members_head;
            while(current_members!=NULL){
                dec_one_from_member(em,current_members);
                current_members=current_members->next;
            }
            prev->next=current->next;
            free(current->name);
            dateDestroy(current->date);
            free(current);
            em->counter_num_of_events--;
            free_element(element);
            free_element(current1);
            return EM_SUCCESS;
        }
        prev=prev->next;
        current=current->next;
    }

    return EM_EVENT_NOT_EXISTS;


}



EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(date_cmp(new_date,em->begginig_date) > 0)
    {
        return EM_INVALID_DATE;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(there_is_event_the_same(em->head_events,event_id,new_date))
    {
        return EM_EVENT_ALREADY_EXISTS;
    }
    Node current=em->head_events;
    while(current!=NULL)
    {

        if(current->id == event_id)
        {
            Event_element element=eventElement_create(current->name,current->id,current->counter);
            Event_element el2=Get_element(em->queue,element);
            element->members_head=copy_Node(el2->members_head);
            free_element(el2);
            pqChangePriority(em->queue,element,
                             current->date, new_date);
            dateDestroy(current->date);
            current->date = dateCopy(new_date);
            free_element(element);
            return EM_SUCCESS;
        }
        current=current->next;
    }
    return EM_EVENT_ID_NOT_EXISTS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id)
{
    if(em==NULL||member_name==NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id<0)
    {
        return EM_INVALID_MEMBER_ID;
    }
    Node current=em->members_in_sysem_head;
    if(em->members_in_sysem_head->name==NULL){
        char * new_name=malloc(sizeof(char)*(strlen(member_name)+1));
        strcpy(new_name,member_name);
        em->members_in_sysem_head->name=new_name;
        em->members_in_sysem_head->id=member_id;
        em->members_in_sysem_head->counter=0;
        if(em->members_in_sysem_head->date)
            dateDestroy(em->members_in_sysem_head->date);
        em->members_in_sysem_head->date=NULL;
        em->members_in_sysem_head->next=NULL;
        return EM_SUCCESS;
    }
    while(current!=NULL){
        if(current->id==member_id){
            return EM_MEMBER_ID_ALREADY_EXISTS;
        }
        current=current->next;
    }
    Node new_mem=createNode(member_name,member_id,0,em->begginig_date);
    new_mem->next=em->members_in_sysem_head;
    em->members_in_sysem_head=new_mem;
    //em->members_in_sysem_head->counter++;
    return EM_SUCCESS;
}


EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id)
{
    if(em == NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(event_id < 0)
    {
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0)
    {
        return EM_INVALID_MEMBER_ID;
    }

    Node cur=em->head_events;
    while(cur!=NULL){
        if(cur->id==event_id)break;
        cur=cur->next;
    }
    if(cur==NULL){
        return EM_EVENT_ID_NOT_EXISTS;
    }


    Node current = em->members_in_sysem_head;
    while(current!=NULL){
        if(current->id==member_id)break;
        current=current->next;
    }
    if(current==NULL){
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    Node current_member_add=createNode(current->name,current->id,current->counter,current->date);
    Node current_event=em->head_events;
    while(current_event!=NULL){
        if(current_event->id==event_id){
            Event_element element=eventElement_create(current_event->name,current_event->id,current_event->counter);
            Event_element wanted= Get_element(em->queue,element);


            Node current1 = wanted->members_head;
            while(current1!=NULL){

                if(current1->id==member_id){
                    free_element(element);
                    free_element(wanted);
                    Destroy_Node(current_member_add);
                    return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
                }
                current1=current1->next;
            }


            Date date=dateCopy(current_event->date);
            pqRemoveElement(em->queue,element);
            //       Node current_member=wanted->members_head;

            current_member_add->next=wanted->members_head;
            wanted->members_head=current_member_add;
            pqInsert(em->queue, wanted, date);
            current->counter++;
            // Destroy_Node(current_member_add);
            free_element(element);
            free_element(wanted);
            dateDestroy(date);
            return EM_SUCCESS;


        }
        current_event=current_event->next;
    }


    return EM_EVENT_ID_NOT_EXISTS;
}

/**
* remove_member_from_event_aux: removes a member from an event.
*
* @param em - the queue that contains the event we want to remove member from.
* @param member_id - the id of the member we want to remove.
* @param current_event - the event we want to remove the member from.
* @param member_in_sys - member in the system.
*/
static void remove_member_from_event_aux(EventManager em,int member_id,Node current_event,Node member_in_sys)
{
    Event_element element = eventElement_create(current_event->name, current_event->id, current_event->counter);
    Event_element wanted = Get_element(em->queue, element);
    Date date = dateCopy(current_event->date);
    pqRemoveElement(em->queue, element);
    Node current_member = wanted->members_head;

    if (current_member->id == member_id) {
        wanted->members_head = current_member->next;
        free(current_member->name);
        dateDestroy(current_member->date);
        free(current_member);
        pqInsert(em->queue, wanted, date);
        free_element(element);
        free_element(wanted);
        dateDestroy(date);
        member_in_sys->counter--;
        return;
    }
    Node prev_current_mem=current_member;
    current_member=current_member->next;
    while (current_member != NULL) {
        if (current_member->id == member_id) {
            prev_current_mem->next = current_member->next;
            free(current_member->name);
            dateDestroy(current_member->date);
            free(current_member);
            free_element(element);
            pqInsert(em->queue, wanted, date);
            dateDestroy(date);
            member_in_sys->counter--;
            free_element(wanted);
            return;
        }
        prev_current_mem = prev_current_mem->next;
        current_member = current_member->next;

    }
}

/**
* find_member_in_event: check if a member is in an event.
*
* @param em - the queue that contains the event we are looking for.
* @param member_id - the id of the member we want to find.
* @param event_id - the event id that we want to find the member in.
* @param name - name of element.
* @return
* TRUE if member is found .
* otherwise FALSE.
*/
static bool find_member_in_event(EventManager em,int member_id,int event_id,char* name)
{
    Event_element element = eventElement_create(name,event_id, 0);
    Event_element wanted = Get_element(em->queue, element);

    Node current_member=wanted->members_head;
    while(current_member!=NULL){
        if(current_member->id==member_id){
            free_element(element);
            free_element(wanted);
            return true;
        }
        current_member=current_member->next;
    }
    free_element(element);
    free_element(wanted);
    return false;
}

EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id)
{
    if(em==NULL)
    {
        return EM_NULL_ARGUMENT;
    }
    if(member_id<0)
    {
        return EM_INVALID_MEMBER_ID;
    }
    if(event_id<0)
    {
        return EM_INVALID_EVENT_ID;
    }
    Node current=em->members_in_sysem_head;
    while(current!=NULL){
        if(current->id==member_id)break;
        current=current->next;
    }
    if(current==NULL){
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    Node current_event=em->head_events;
    while(current_event!=NULL){
        if(current_event->id==event_id){
            if(!find_member_in_event(em,member_id,event_id,current_event->name))
            {
                return EM_EVENT_AND_MEMBER_NOT_LINKED;
            }
            remove_member_from_event_aux(em,member_id,current_event,current);
            return EM_SUCCESS;
        }
        current_event=current_event->next;
    }

    return EM_EVENT_ID_NOT_EXISTS;
}




/**
* bring_date: brings the date.
*
* @param element - element that has date.
* @param head_events - all the events.
* @return
* returns NULL.
*/
static Date bring_date( Event_element element,Node head_events)
{
    Node current_event=head_events;
    while(current_event!=NULL){
        if(current_event->id==element->id)
            return dateCopy(current_event->date);
        current_event=current_event->next;
    }
    return NULL;

}

/**
* Remove_from_node: remove element from the node.
*
* @param head_events - events we want to remove element from.
* @param element - element we want to remove.
* @return
* returns events without the element we removed.
*/
static Node Remove_from_node(Node head_events, Event_element element)
{
    if(element->id==head_events->id){
        if(head_events->next==NULL){
            Node current=head_events;
            dateDestroy(current->date);
            free(current->name);
            current->name=NULL;
            current->date=NULL;
            current->id=0;
            current->counter=0;

            //free(current);
            return head_events;
        }
        else{

            Node current=head_events;
            head_events=head_events->next;
            dateDestroy(current->date);
            free(current->name);
            //  current->name=NULL;
            // current->date=NULL;
            free(current);
            return head_events;
        }
    }
    Node current=head_events->next;
    Node prev=head_events;
    while(current!=NULL){
        if(current->id==element->id){
            prev->next=current->next;
            dateDestroy(current->date);
            free(current->name);
            free(current);
            break;
        }
        prev=prev->next;
        current=current->next;
    }
    return head_events;

}



EventManagerResult emTick(EventManager em, int days)
{
    if(em==NULL)
        return EM_NULL_ARGUMENT;
    if(days<=0){
        return EM_INVALID_DATE;
    }
    for(int i=0;i<days;i++)
        dateTick(em->begginig_date);

    Event_element current=pqGetFirst(em->queue);
    Date current_date=NULL;
    Node current_Event=em->head_events;


    while(current_Event!=NULL){
        if(current->id==current_Event->id){
            current_date=dateCopy(current_Event->date);
            if(date_cmp(em->begginig_date,current_Event->date)>0){
                if(current_date){
                    dateDestroy(current_date);
                }

                return EM_SUCCESS;
            }
        }
        current_Event=current_Event->next;
    }

    while(current&&date_cmp(em->begginig_date,current_date)<0){
        em->head_events=Remove_from_node(em->head_events,current);
        Node current_members=current->members_head;
        while(current_members!=NULL){
            dec_one_from_member(em,current_members);
            current_members=current_members->next;
        }
        pqRemoveElement(em->queue, current);
        em->counter_num_of_events--;
        current=pqGetFirst(em->queue);
        if(current){
            if(current_date){
                dateDestroy(current_date);
            }

            current_date=bring_date(current,em->head_events);}



    }
    if(current_date){dateDestroy(current_date);}

    return EM_SUCCESS;


}

/**
* if_still_in_p: check if elements still in queue.
*
* @param element1 - first element.
* @param element2 - second element.
* @param node - the node to check in.
* @return
* FALSE - if one of the arguments is NULL or is not still in p.
* otherwise TRUE.
*/
static bool if_still_in_p( Event_element element1,Event_element element2,Node node)
{
    if(element1==NULL||element2==NULL) {
        return false;
    }
    Node current1=node;
    while(current1!=NULL){
        if(element1->id==current1->id)break;
        current1=current1->next;
    }
    Node current2=node;
    while(current2!=NULL){
        if(element2->id==current2->id)break;
        current2=current2->next;
    }
    if(current1!=NULL&&current2!=NULL){
        if(dateCompare(current1->date,current2->date)==0){
            return true;
        }
    }

    return false;


}

int emGetEventsAmount(EventManager em)
{
    if(!em){
        return -1;
    }
    return em->counter_num_of_events;
}

char* emGetNextEvent(EventManager em)
{
    // char* c=NULL;
    if(!em) {
        return NULL;
    }
    if(em->counter_num_of_events==0) {
        return NULL;
    }
    Event_element element_check=pqGetFirst(em->queue);
    if(!element_check) {
        return NULL;
    }

    return element_check->name;
}


/**
* print_members: print the members.
*
* @param fid - the given file.
* @param node_head - node we want to print.
*/
static void print_members(FILE* fid,Node node_head)
{
    if (node_head==NULL){
        return;
    }
    int max_id=0;
    Node current=node_head;
    while(current!=NULL){
        if(current->id>max_id)max_id=current->id;
        current=current->next;
    }
    char * members_names[max_id+1];
    for (int i=0;i<max_id+1;i++)members_names[i]=NULL;

    current=node_head;
    while(current!=NULL){
        char* new_name=  malloc(sizeof(char)*strlen(current->name)+1);
        strcpy(new_name,current->name);
        members_names[current->id]=new_name;
        current=current->next;
    }
    for (int i=0;i<max_id+1;i++) {
        if (members_names[i] != NULL) {

            //printf(",%s", members_names[i]);
            fprintf(fid, ",%s", members_names[i]);
        }

    }
    fprintf( fid, "\n");
    for(int i=0;i<max_id+1;i++){
        if(members_names[i]){
            free(members_names[i]);
        }
    }
    //free(members_names);
}



/**
* print_members_by_counter: print the members but now according to the counter.
*
* @param fid - the given file.
* @param node_head - node we want to print.
* @param counter - counter we want to print according to.
*/
static void print_members_by_counter(FILE* fid,Node node_head,int counter)
{
    if(!node_head){
        return;
    }
    if(!node_head->name){
        return;
    }
    int max_id=0;
    Node current=node_head;
    while(current!=NULL){
        if(current->id>max_id)max_id=current->id;
        current=current->next;
    }
    char *members_names[max_id+1];
    for (int i=0;i<max_id+1;i++)members_names[i]=NULL;

    current=node_head;
    while(current!=NULL){
        char* new_name=malloc(sizeof(char)*strlen(current->name)+1);
        strcpy(new_name,current->name);
        members_names[current->id]=new_name;
        current=current->next;
    }
    for (int i=0;i<max_id+1;i++){
        if(members_names[i]==NULL)continue;
        fprintf(fid,"%s,%d\n",members_names[i], counter);
    }
    for (int i=0;i<max_id+1;i++){
        if(members_names[i]){free(members_names[i]);}

    }
//free(members_names);
}

void emPrintAllEvents(EventManager em, const char* file_name)
{
    int counter_tot = 0;
    FILE* fid;
    PriorityQueue queue_for_func =pqCopy(em->queue);
    fid=fopen(file_name,"w");
    Event_element element_lift =copy_element(pqGetFirst(queue_for_func)) ;
    counter_tot=element_lift->counter;
    Event_element element_right =copy_element( pqGetNext(queue_for_func));
    while (pqGetSize(queue_for_func) != 0&&element_lift!=NULL){
        if (if_still_in_p(element_lift, element_right,em->head_events)) {

            if (element_right->counter < counter_tot)counter_tot = element_right->counter;
            free_element(element_right);
            element_right = copy_element(pqGetNext(queue_for_func));

        }
        else {
            Node current=em->head_events;
            while(current!=NULL){
                if(current->counter==counter_tot)break;
                current=current->next;
            }

            if(!current){
                return;
            }
            int day=0;
            int month=0;
            int year=0;

            dateGet(current->date,&day,&month,&year);

            fprintf(fid,"%s,%d.%d.%d",current->name,day,month,year);
            Event_element element=eventElement_create(current->name,current->id,current->counter);
            Event_element wanted=Get_element(queue_for_func,element);

            print_members(fid, wanted->members_head);
            if(!wanted->members_head){
                fprintf( fid, "\n");
            }
            pqRemoveElement(queue_for_func,element);
            free_element(wanted);
            free_element(element);

            free_element(element_lift);
            if(element_right){
                free_element(element_right);
            }

            element_lift =copy_element(pqGetFirst(queue_for_func));
            element_right = copy_element(pqGetNext(queue_for_func));
            if(element_lift){
                counter_tot=element_lift->counter;}
        }


    }
    fclose(fid);
    if(element_lift){
        free_element(element_lift);
    }
    if(element_right){
        free_element(element_right);
    }
    pqDestroy(queue_for_func);
}


void emPrintAllResponsibleMembers(EventManager em, const char* file_name)
{
    FILE* fid=fopen(file_name,"w");
    Node hash[em->counter_num_of_events+1];
    for(int i=0;i<em->counter_num_of_events+1;i++)hash[i]=NULL;
    Node current=em->members_in_sysem_head;
    if(!em->members_in_sysem_head->name){
        fclose(fid);
        return;
    }
    while(current!=NULL){
        Node   current_copied=createNode(current->name,current->id,current->counter,NULL);
        current_copied->next=hash[current->counter];
        hash[current->counter]=current_copied;
        current=current->next;

    }
    for(int i=em->counter_num_of_events;i>0;i--) {
        if(hash[i]){  print_members_by_counter(fid, hash[i], i);}

    }

    fclose(fid);


    for(int i=0;i<em->counter_num_of_events+1;i++){
        if(hash[i]){
            Destroy_Node(hash[i]);
        }
    }
}