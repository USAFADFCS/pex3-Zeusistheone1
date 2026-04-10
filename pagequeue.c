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

    if (pq->size = 0){ //if pq is empty (like at first)
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        if(!newNode) return;
        newNode->next = NULL;
        newNode->pageNum = pageNum;
        newNode->prev = NULL;

        pq->head = newNode;
        pq->tail = newNode;
        pq->size += 1;
    }

    else{
        int index = 0;
        PqNode* current = pq->tail; //pointer to current index of pq
        for(int i = 0; i < pq->size; i++){
            index = pq->size - i; //for tail first seach
            if(current->pageNum == pageNum){ //HIT

                //first, unlink current node
                if(current->prev){
                    current->prev->next = current->next;
                }
                else{
                    pq->head = current->next;
                }
                if(current->next){
                    current->next->prev = current->prev;
                }
                else{
                    pq->tail = current->prev;
                }

                //append current to tail
                pq->tail->next = current;

                //return index in pq
                return index;
            }
            current = current->prev; //should work. However, may go out of bounds if i goes all the way to 0.
        }
        //MISS
        //make new node, append to tail
        PqNode* newNode = (PqNode*)malloc(sizeof(PqNode));
        if(!newNode) return;
        newNode->next = NULL;
        newNode->pageNum = pageNum;
        newNode->prev = pq->tail;

        pq->tail->next = newNode;
        pq->tail = newNode;
        pq->size += 1;

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
        printf("%d->", current->pageNum);
        current = current->next;
    }
    printf("tail\n");
}
