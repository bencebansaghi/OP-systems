#include <stdio.h>
#include <stdlib.h>

#define MAX_MEMORY 640 // Maximum memory size in KB

typedef struct Partition {
    int size;
    int startAddress;
    struct Partition *next;
} Partition;

Partition* initializeFreePartitions() {
    Partition *freeList = (Partition*)malloc(sizeof(Partition));
    freeList->size = MAX_MEMORY;
    freeList->startAddress = 0;
    freeList->next = NULL;
    return freeList;
}

void displayFreePartitions(Partition *freeList) {
    while (freeList != NULL) {
        printf("[%d, %d] -> ", freeList->startAddress, freeList->size+freeList->startAddress);
        freeList = freeList->next;
    }
    printf("NULL\n");
}

Partition* allocFirstFit(Partition *freeList, int size) {
    Partition *prev = NULL;
    Partition *currentFree = freeList;
    while (currentFree != NULL) {
        if (currentFree->size >= size) {
            Partition *newPartition = (Partition*)malloc(sizeof(Partition));
            newPartition->size = size;
            newPartition->startAddress = currentFree->startAddress;
            
            // Update free partition chain
            currentFree->startAddress += size;
            currentFree->size -= size;
            if (currentFree->size == 0) {
                if (prev == NULL)
                    freeList = currentFree->next;
                else
                    prev->next = currentFree->next;
                free(currentFree);
            }
            return newPartition;
        }
        prev = currentFree;
        currentFree = currentFree->next;
    }
    return NULL;
}

Partition* addNewPartition(Partition *freeList, int size){
    Partition *newPartition = allocFirstFit(freeList, size);
    if (newPartition == NULL) {
        printf("\nNo free partition available for allocation of size %d\n", size);
        return NULL;
    }
    else {
        printf("\nAllocated new partition: [%d, %d]\n", newPartition->startAddress, newPartition->startAddress + newPartition->size);
        printf("Free chain after allocation of new partition: ");
        displayFreePartitions(freeList);
        return newPartition;
    }
}

void freeMemory(Partition **freeList, Partition *block) {
    Partition *prev = NULL;
    Partition *current = *freeList;

    while (current != NULL && current->startAddress < block->startAddress) {
        prev = current;
        current = current->next;
    }
    printf("\nFreeing partition: [%d, %d]\n", block->startAddress, block->startAddress + block->size);
    
    // Merge with previous partition if possible
    if (prev != NULL && prev->startAddress + prev->size == block->startAddress) {
        prev->size += block->size;
        free(block);
        block = prev;
    } else {
        block->next = current;
        *freeList = block;
    }

    // Merge with next partition if possible
    if (current != NULL && block->startAddress + block->size == current->startAddress) {
        block->size += current->size;
        block->next = current->next;
        free(current);
    }
    printf("Free chain after freeing the partition: ");
    displayFreePartitions(*freeList);
}

int main() {
    Partition *freeList = initializeFreePartitions();

    printf("Initial Status:\n");
    displayFreePartitions(freeList);

    Partition *allocated1 = addNewPartition(freeList, 130);
    Partition *allocated2 = addNewPartition(freeList, 60);
    Partition *allocated3 = addNewPartition(freeList, 100);
    freeMemory(&freeList, allocated2);
    Partition *allocated4 = addNewPartition(freeList, 200);
    freeMemory(&freeList, allocated3);
    freeMemory(&freeList, allocated1);
    Partition *allocated5 = addNewPartition(freeList, 140);
    Partition *allocated6 = addNewPartition(freeList, 60);
    Partition *allocated7 = addNewPartition(freeList, 50);
    freeMemory(&freeList, allocated6);

    // Freeing leftover  - not part of task
    while (freeList != NULL) {
        Partition *temp = freeList;
        freeList = freeList->next;
        free(temp);
    }
    free(allocated4);
    free(allocated5);
    free(allocated7);
    return 0;
}
