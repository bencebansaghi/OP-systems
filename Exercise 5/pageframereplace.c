// Removed (seemingly) unnecessary include stdafx.h since neither the file was given nor used in any part of the code.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define VM_PAGE 7       /*Number of virtual pages*/
#define PM_PAGE 4       /* Number of memory blocks allocated to the job */
#define TOTAL_INSERT 18 /*Number of virtual page replacements*/
typedef struct{
    int vmn;   // virtual page number
    int pmn;   // physical page number
    int exist; // Whether to occupy a physical block
    int time;
} vpage_item;
vpage_item page_table[VM_PAGE];
vpage_item *ppage_bitmap[PM_PAGE];
int vpage_arr[TOTAL_INSERT] = {1, 2, 3, 4, 2, 6, 2, 1, 2, 3, 7, 6, 3, 2, 1, 2, 3, 6}; // The access order virtual pages
void init_data() // initialize data
{
    for (int i = 0; i < VM_PAGE; i++)
    {
        page_table[i].vmn = i + 1;
        page_table[i].pmn = -1;
        page_table[i].exist = 0;
        page_table[i].time = -1;
    }
    for (int i = 0; i < PM_PAGE; i++) /*initialize the physical page map*/
    {
        ppage_bitmap[i] = NULL;
    }
    printf("The virtual page access order is: ");
    for (int i = 0; i < TOTAL_INSERT; i++)
    {
        printf("%d ", vpage_arr[i]);
    }
    printf("\n");
}
void FIFO()
{
    int k = 0;
    int i;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    bool isleft = true; /* Whether there are remaining physical blocks */
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            if (k < PM_PAGE)
            {
                if (ppage_bitmap[k] == NULL) /*find a free block*/
                {
                    ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[k]->exist = 1;
                    ppage_bitmap[k]->pmn = k;
                    ppage_bitmap[k]->time = current_time;
                    printf("Added page %d to physical block %d\n", ppage_bitmap[k]->vmn, k);
                    k++;
                }
            }
            else
            {
                int temp = ppage_bitmap[0]->time;
                int j = 0;
                for (i = 0; i < PM_PAGE; i++)
                {
                    if (ppage_bitmap[i]->time < temp)
                    {
                        temp = ppage_bitmap[i]->time;
                        j = i;
                    }
                }
                printf("Replaced page %d with page %d in physical block %d\n", ppage_bitmap[j]->vmn, vpage_arr[sum], j);
                ppage_bitmap[j]->exist = 0;
                ppage_bitmap[j] = &page_table[vpage_arr[sum] - 1]; /*update page table*/
                ppage_bitmap[j]->exist = 1;
                ppage_bitmap[j]->pmn = j;
                ppage_bitmap[j]->time = current_time;
            }
        }
        else{
            printf("Page %d already exists in physical block %d\n", vpage_arr[sum], page_table[vpage_arr[sum] - 1].pmn);
        }
        current_time++;
        sum++;
    }
    printf("The number of page fault(s) of FIFO is: %d\t Page fault rate: %f\t Number of replacement(s) : % d\tReplacement rate : % f \n", missing_page_count, missing_page_count / (float)TOTAL_INSERT,missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}
void LRU() 
{
    int k = 0;
    int i;
    int sum = 0;
    int missing_page_count = 0;
    int current_time = 0;
    bool isleft = true;
    while (sum < TOTAL_INSERT)
    {
        if (page_table[vpage_arr[sum] - 1].exist == 0)
        {
            missing_page_count++;
            if (k < PM_PAGE)
            {
                if (ppage_bitmap[k] == NULL) /*find a free block*/
                {
                    ppage_bitmap[k] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[k]->exist = 1;
                    ppage_bitmap[k]->pmn = k;
                    printf("Added page %d to physical block %d\n", ppage_bitmap[k]->vmn, k);
                    ppage_bitmap[k]->time = current_time;
                    k++;
                }
            }
            else
            {
                int temp = ppage_bitmap[0]->time;
                int j = 0;
                for (i = 0; i < PM_PAGE; i++)
                {
                    if (ppage_bitmap[i]->time < temp)
                    {
                        temp = ppage_bitmap[i]->time;
                        j = i;
                    }
                }
                printf("Replaced page %d with page %d in physical block %d\n", ppage_bitmap[j]->vmn, vpage_arr[sum], j);
                ppage_bitmap[j]->exist = 0;
                ppage_bitmap[j] = &page_table[vpage_arr[sum] - 1]; /*update page table*/
                ppage_bitmap[j]->exist = 1;
                ppage_bitmap[j]->pmn = j;
                ppage_bitmap[j]->time = current_time;
            }
        }
        else{
            printf("Page %d already exists in physical block %d, updating time accessed\n", vpage_arr[sum], page_table[vpage_arr[sum] - 1].pmn);
            page_table[vpage_arr[sum] - 1].time = current_time; // This is the only difference between FIFO and LRU
        }
        current_time++;
        sum++;
    }
    printf("The number of page fault(s) of LRU is: %d\t Page fault rate: %f\t Number of replacement(s) : % d\tReplacement rate : % f \n", missing_page_count, missing_page_count / (float)TOTAL_INSERT,missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

void OPT() {
    int sum = 0;
    int missing_page_count = 0;
    int replacements = 0;
    while (sum < TOTAL_INSERT) {
        if (page_table[vpage_arr[sum] - 1].exist == 0) {
            missing_page_count++;
            int opt = -1, latest_use = -1;
            for (int i = 0; i < PM_PAGE; i++) {
                if (ppage_bitmap[i] == NULL) {
                    ppage_bitmap[i] = &page_table[vpage_arr[sum] - 1];
                    ppage_bitmap[i]->exist = 1;
                    ppage_bitmap[i]->pmn = i;
                    printf("Added page %d to physical block %d\n", ppage_bitmap[i]->vmn, i);
                    opt = -1;
                    break;
                } else {
                    // Find the next use of the current physical page
                    int next_use = TOTAL_INSERT + 1;
                    for (int j = sum + 1; j < TOTAL_INSERT; j++) {
                        if (vpage_arr[j] == ppage_bitmap[i]->vmn) {
                            next_use = j;
                            printf("Page %d will be used next at %d\n", ppage_bitmap[i]->vmn, next_use);
                            break;
                        }
                    }
                    // Update the optimal page to replace if the current page has the latest use
                    if (next_use > latest_use) {
                        latest_use = next_use;
                        opt = i;
                    }
                }
            }
            if (opt != -1) {
                printf("Replaced page %d with page %d in physical block %d, since this is the last accessed\n", ppage_bitmap[opt]->vmn, vpage_arr[sum], opt);
                replacements++;
                ppage_bitmap[opt]->exist = 0;
                ppage_bitmap[opt] = &page_table[vpage_arr[sum] - 1];
                ppage_bitmap[opt]->exist = 1;
                ppage_bitmap[opt]->pmn = opt;
            }
        } else {
            printf("Page %d was accessed, but no page fault occurred.\n", vpage_arr[sum]);
        }
        sum++;
    }
    printf("The number of page fault(s) of OPT is: %d\t Page fault rate: %f\t Number of replacement(s) : % d\tReplacement rate : % f \n", missing_page_count, missing_page_count / (float)TOTAL_INSERT,missing_page_count - PM_PAGE, (missing_page_count - PM_PAGE) / (float)TOTAL_INSERT);
}

int main()
{
    int a=-1;
    do
    {
        printf("Please choose page replacement algorithm：1.FIFO, 2.LRU, 3.OPT, 0. quit:\t");
        scanf("%d", &a); // Fixed 'scanf_s' to 'scanf'
        if (a != 0)
            init_data();
        switch (a)
        {
        case 1:
            FIFO();
            break;
        case 2:
            LRU();
            break;
        case 3:
            OPT();
            break;
        }
        printf("\n");
    }while (a != 0);
    printf("Quitting...\n");
    return 0;
}
