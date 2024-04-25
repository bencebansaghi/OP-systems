#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAXJOB 50
// const was changed to define cause it was not working

typedef struct node
{
    int number;      // job number
    int reach_time;  // arrival time
    int need_time;   // execution time
    int privilege;   // priority
    float excellent; // response ratio
    int start_time;  // start time
    int wait_time;   // waiting time
    int visited;     // if the job is accessed
    bool isreached;  // if the job has arrived
} job;

job jobs[MAXJOB]; // job sequence
int quantity;     // number of jobs

void initial_jobs()
{
    int i;
    for (i = 0; i < MAXJOB; i++)
    {
        jobs[i].number = 0;
        jobs[i].reach_time = 0;
        jobs[i].privilege = 0;
        jobs[i].excellent = 0.0;
        jobs[i].start_time = 0;
        jobs[i].wait_time = 0;
        jobs[i].visited = 0;
        jobs[i].isreached = false;
    }
    quantity = 0;
}
void reset_jinfo()
{
    int i;
    for (i = 0; i < MAXJOB; i++)
    {
        jobs[i].excellent = 0.0;
        jobs[i].start_time = 0;
        jobs[i].wait_time = 0;
        jobs[i].visited = 0;
        jobs[i].isreached = false;
    }
}
int findminjob()
{
    int minjob = -1;
    int minloc = -1;
    for (int i = 0; i < quantity; i++)
    {
        if (minloc == -1)
        {
            if (jobs[i].isreached == true && jobs[i].visited == 0)
            {
                minjob = jobs[i].need_time;
                minloc = i;
            }
        }
        else if (minjob > jobs[i].need_time && jobs[i].visited == 0 && jobs[i].isreached == true)
        {
            minjob = jobs[i].need_time;
            minloc = i;
        }
    }
    return minloc;
}
int findrearlyjob()
{
    int rearlyloc = -1;
    int rearlyjob = -1;
    for (int i = 0; i < quantity; i++)
    {
        if (rearlyloc == -1)
        {
            if (jobs[i].visited == 0)
            {
                rearlyloc = i;
                rearlyjob = jobs[i].reach_time;
            }
        }
        else if (rearlyjob > jobs[i].reach_time && jobs[i].visited == 0)
        {
            rearlyjob = jobs[i].reach_time;
            rearlyloc = i;
        }
    }
    return rearlyloc;
}

void updateratio(int time)
{
    for (int i = 0; i < quantity; i++)
    {
        if (jobs[i].isreached == true && jobs[i].visited == 0)
        {
            float turnaround = time - jobs[i].reach_time;
            float needtime = jobs[i].need_time;
            float excellent = turnaround / needtime;
            jobs[i].excellent = excellent;
        }
    }
}

int findmaxratio()
{
    float maxratio = -1;
    int maxloc = -1;
    for (int i = 0; i < quantity; i++)
    {
        if (maxloc == -1)
        {
            if (jobs[i].isreached == true && jobs[i].visited == 0)
            {
                maxratio = jobs[i].excellent;
                maxloc = i;
            }
        }
        else if (maxratio < jobs[i].excellent && jobs[i].visited == 0 && jobs[i].isreached == true)
        {
            maxratio = jobs[i].excellent;
            maxloc = i;
        }
    }
    return maxloc;
}

void check_reached(int time)
{
    for (int i = 0; i < quantity; i++)
    {
        if (jobs[i].reach_time <= time)
        {
            jobs[i].isreached = true;
        }
    }
}
int getlowestprivilegeid()
{
    int minprivilege = -1;
    int minloc = -1;
    for (int i = 0; i < quantity; i++)
    {
        if (minloc == -1)
        {
            if (jobs[i].visited == 0)
            {
                minprivilege = jobs[i].privilege;
                minloc = i;
            }
        }
        else if (minprivilege > jobs[i].privilege && jobs[i].visited == 0)
        {
            minprivilege = jobs[i].privilege;
            minloc = i;
        }
    }
    return minloc;
}
// read all job data
void readJobdata()
{
    FILE *fp;
    char fname[20];
    int i;
    printf("please input job data file name: ");
    scanf("%s", fname);
    if ((fp = fopen(fname, "r")) == NULL)
    {
        printf("error, open file failed, please check filename:\n");
    }
    else
    {
        while (!feof(fp))
        {
            if (fscanf(fp, "%d %d %d %d", &jobs[quantity].number, &jobs[quantity].reach_time, &jobs[quantity].need_time, &jobs[quantity].privilege) == 4)
                quantity++;
        }
        printf("output the origin job data\n");
        printf("---------------------------------------------------------\n");
        printf("\tjobID\treachtime\tneedtime\tprivilege\n");
        for (i = 0; i < quantity; i++)
        {
            printf("\t%-8d\t%-8d\t%-8d\t%-8d\n", jobs[i].number, jobs[i].reach_time, jobs[i].need_time, jobs[i].privilege);
        }
    }
}

void FCFS()
{
    int i;
    int current_time = 0;
    int loc;
    int total_waitime = 0;
    int total_roundtime = 0;
    loc = findrearlyjob();
    printf("\n\nFCFS job stream\n");
    printf("--------------------------------------------------------------------------\n");
    printf("\tjobID\treachtime\tstarttime\twaittime\troundtime\n");
    current_time = jobs[loc].reach_time;
    for (i = 0; i < quantity; i++)
    {
        if (jobs[loc].reach_time > current_time)
        {
            jobs[loc].start_time = jobs[loc].reach_time;
            current_time = jobs[loc].reach_time;
        }
        else
        {
            jobs[loc].start_time = current_time;
        }
        jobs[loc].wait_time = current_time - jobs[loc].reach_time;
        printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n ", loc + 1, jobs[loc].reach_time, jobs[loc].start_time, jobs[loc].wait_time,
               jobs[loc].wait_time + jobs[loc].need_time);
        jobs[loc].visited = 1;
        current_time += jobs[loc].need_time;
        total_waitime += jobs[loc].wait_time;
        total_roundtime = total_roundtime + jobs[loc].wait_time + jobs[loc].need_time;
        loc = findrearlyjob();
    }
    printf("total waiting time:%-8d total turnaround time:%-8d\n", total_waitime,
           total_roundtime);
    printf("average waiting time: %4.2f average turnaround time: %4.2f\n",
           (float)total_waitime / (quantity), (float)total_roundtime / (quantity));
}

void SJF()
{
    int i;
    int current_time = 0;
    int loc;
    int total_waitime = 0;
    int total_roundtime = 0;
    loc = findrearlyjob();
    printf("\n\nSJF job stream\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("\tjobID\treachtime\tneedtime\tstarttime\twaittime\troundtime\n");
    current_time = jobs[loc].reach_time;
    for (i = 0; i < quantity; i++)
    {
        if (jobs[loc].reach_time > current_time)
        {
            jobs[loc].start_time = jobs[loc].reach_time;
            current_time = jobs[loc].reach_time;
        }
        else
        {
            jobs[loc].start_time = current_time;
        }
        jobs[loc].wait_time = current_time - jobs[loc].reach_time;
        printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n ", loc + 1, jobs[loc].reach_time,jobs[loc].need_time, jobs[loc].start_time, jobs[loc].wait_time,
               jobs[loc]
                       .wait_time +
                   jobs[loc].need_time);
        jobs[loc].visited = 1;
        current_time += jobs[loc].need_time;
        total_waitime += jobs[loc].wait_time;
        total_roundtime = total_roundtime + jobs[loc].wait_time + jobs[loc].need_time;
        check_reached(current_time);
        loc = findminjob();
        if (loc==-1)
        {
            loc=findrearlyjob();
        }
    }
    printf("total waiting time:%-8d total turnaround time:%-8d\n", total_waitime,
           total_roundtime);
    printf("average waiting time: %4.2f average turnaround time: %4.2f\n",
           (float)total_waitime / (quantity), (float)total_roundtime / (quantity));
}

void HRRF()
{
    int i;
    int current_time = 0;
    int loc;
    int total_waitime = 0;
    int total_roundtime = 0;
    loc = findrearlyjob();
    printf("\n\nHRRF job stream\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("\tjobID\treachtime\tresponseratio\tstarttime\twaittime\troundtime\n");
    current_time = jobs[loc].reach_time;
    for (i = 0; i < quantity; i++)
    {
        if (jobs[loc].reach_time > current_time)
        {
            jobs[loc].start_time = jobs[loc].reach_time;
            current_time = jobs[loc].reach_time;
        }
        else
        {
            jobs[loc].start_time = current_time;
        }
        jobs[loc].wait_time = current_time - jobs[loc].reach_time;
        printf("\t%-8d\t%-8d\t%4.2f\t\t%-8d\t%-8d\t%-8d\n ", loc + 1, jobs[loc].reach_time, jobs[loc].excellent, jobs[loc].start_time, jobs[loc].wait_time,
               jobs[loc]
                       .wait_time +
                   jobs[loc].need_time);
        jobs[loc].visited = 1;
        current_time += jobs[loc].need_time;
        total_waitime += jobs[loc].wait_time;
        total_roundtime = total_roundtime + jobs[loc].wait_time + jobs[loc].need_time;
        check_reached(current_time);
        updateratio(current_time);
        loc = findmaxratio();
        if (loc==-1)
        {
            loc=findrearlyjob();
        }
    }
    printf("total waiting time:%-8d total turnaround time:%-8d\n", total_waitime,
           total_roundtime);
    printf("average waiting time: %4.2f average turnaround time: %4.2f\n",
           (float)total_waitime / (quantity), (float)total_roundtime / (quantity));
}

void HPF()
{
    int i;
    int current_time = 0;
    int loc;
    int total_waitime = 0;
    int total_roundtime = 0;
    loc = findrearlyjob();
    printf("\n\nHPF job stream\n");
    printf("-----------------------------------------------------------------------------------------\n");
    printf("\tjobID\treachtime\tprivilege\tstarttime\twaittime\troundtime\n");
    current_time = jobs[loc].reach_time;
    for (i = 0; i < quantity; i++)
    {
        if (jobs[loc].reach_time > current_time)
        {
            jobs[loc].start_time = jobs[loc].reach_time;
            current_time = jobs[loc].reach_time;
        }
        else
        {
            jobs[loc].start_time = current_time;
        }
        jobs[loc].wait_time = current_time - jobs[loc].reach_time;
        printf("\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\t%-8d\n ", loc + 1, jobs[loc].reach_time, jobs[loc].privilege, jobs[loc].start_time, jobs[loc].wait_time,
               jobs[loc]
                       .wait_time +
                   jobs[loc].need_time);
        jobs[loc].visited = 1;
        current_time += jobs[loc].need_time;
        total_waitime += jobs[loc].wait_time;
        total_roundtime = total_roundtime + jobs[loc].wait_time + jobs[loc].need_time;
        check_reached(current_time);
        loc = getlowestprivilegeid();
    }
    printf("total waiting time:%-8d total turnaround time:%-8d\n", total_waitime,
           total_roundtime);
    printf("average waiting time: %4.2f average turnaround time: %4.2f\n",
           (float)total_waitime / (quantity), (float)total_roundtime / (quantity));
}
int main()
{
    initial_jobs();
    readJobdata();
    FCFS();
    reset_jinfo();
    SJF();
    reset_jinfo();
    HRRF();
    reset_jinfo();
    HPF();
    return 0;
}
