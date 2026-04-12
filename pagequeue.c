/** pagequeue.c
 * ===========================================================
 * Name: Ray Soldini, __ ___ 2026
 * Section: CS483 / M4
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue* pq = (PageQueue*)malloc(sizeof(PageQueue));
    if (!pq) return NULL;
    pq->head = NULL;
    pq->tail = NULL;
    pq->maxSize = maxSize;
    pq->size = 0;
    return pq;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.

    if (pq->size == 0){ //if pq is empty (like at first)
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        if(!newNode) return -1;
        newNode->next = NULL;
        newNode->pageNum = pageNum;
        newNode->prev = NULL;

        pq->head = newNode;
        pq->tail = newNode;
        pq->size += 1;
    }

    else{
        // int d = 0;
        PqNode* current = pq->tail; //pointer to current index of pq
        for(int i = 0; i < pq->size; i++){
            // d = pq->size - i; //for tail first seach
            if(current->pageNum == pageNum){ //HIT

                //first, unlink current node
                // if(current != pq->head){ //if not head
                //     current->prev->next = current->next; //1's next will point to 2's next (could be NULL)
                // }
                // else{
                //     pq->head = current->next; //if current is 1, new head will point to 2
                //     pq->head->prev = NULL;
                // }
                // if(current != pq->tail){ //if not tail
                //     current->next->prev = current->prev; //3's prev will point to 2's prev
                // }
                // else{
                //     pq->tail = current->prev; //if current is 3, new tail will point to 2
                //     pq->tail->next = NULL;
                //     // pq->tail = current;
                // }

                // //append current to tail
                // pq->tail->next = current;
                // current->next = NULL;
                // pq->tail = current;
                if(i == 0){ //if it's tail, do nothing (this includes when there's only 1 node)
                    return i;
                }

                else if(i == pq->size-1){ //if it's head (current points to head node)
                    pq->head = current->next; //current->next should be same as head->next
                    pq->head->prev = NULL; //unlink head
                    pq->tail->next = current; //point from old tail to new tail
                    current->prev = pq->tail; //point from new tail to old tail
                    pq->tail = current; //pq->tail now points to new tail
                    pq->tail->next = NULL; //unlink the end of the new tail
                }
                else{
                    //have 1 and 3 point to each other
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    //move current to back
                    current->prev = pq->tail;
                    current->next = NULL;
                    //make current new tail
                    pq->tail->next = current;
                    pq->tail = current;
                }

                //return index in pq
                return i;
            }
            current = current->prev; //should work. However, may go out of bounds if i goes all the way to 0.
        }
        //MISS
        //make new node, append to tail
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        if(!newNode) return -1;
        newNode->next = NULL;
        newNode->pageNum = pageNum;
        newNode->prev = pq->tail;

        pq->tail->next = newNode;
        pq->tail = newNode;
        pq->size += 1;

        //261616, 291..., 404.., 600
        //if size now exceeds max size
        if(pq->size > pq->maxSize){
            PqNode* evictNode = pq->head;
            pq->head = evictNode->next;
            pq->head->prev = NULL;
            free(evictNode);

            pq->size -= 1;
        }
    }

    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    if(!pq) return;
    PqNode* current = pq->head;
    while(current){
        PqNode* temp = current;
        current = current->next;
        free(temp);
    }
    free(pq);    
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
    if(!pq) return;
    PqNode* current = pq->head;
    printf("Head->");
    while(current){
        printf("%lu->", current->pageNum);
        current = current->next;
    }
    printf("tail\n");
}
