#include <stdio.h>
#include <stdlib.h>

//CPU scheduling - Priority - nonpreemptive
//Processes are numbered from p0,..,pn by default

typedef struct proc {
	int pn;
	int a_time;
	int burst;

	int priority;

	int ta_time;
	int w_time;

	struct proc *next;
} Process;
Process *ps=NULL;
Process p[100];
int n;

void buildProcessList(Process *p, int n) { //create linkedlist  sorted by arrival time
	int i;
	Process *temp;
	for(i=0;i<n;i++) {
		if(ps == NULL) {
			ps = &p[i];
		} else {
			if(ps->a_time > p[i].a_time) { //elem is first elem
				p[i].next = ps;
				ps = &p[i];
				continue; //skip the next section and start with next i
			}
			temp = ps;

			while(temp->next != NULL && p[i].a_time > temp->next->a_time) {
				temp=temp->next;
			}
			if(temp->next != NULL)
				p[i].next = temp->next;

			temp->next = &p[i];
		}
	}
}

void removeProcess(Process *p) {
	Process *temp = ps;
	
	if(p == ps) {
		ps=ps->next;
		return;
	}

	while(temp->next != NULL && temp->next != p) {
		temp=temp->next;
	}
	if(temp->next==NULL) return;

	temp->next = p->next;
}

void displayInformation() {
	Process *temp = ps, *currjob, *temp2;
	int i, mp=0;
	int ct = 0; //current time
	int state = 0; //0- idle, 1- process running
	float sumta=0,sumw=0;

	printf("Gantt chart: \n");
	while(temp != NULL) {
		if(state == 0 && temp->a_time <= ct) {
			
			//find job with minimum priority(change minimum to maximum based on whether smaller number means high priority or vice versa by changing < to >)
			currjob=temp2 = temp;
			mp = temp->priority;
			while(temp2 != NULL && temp2->a_time <= ct) {
				if(temp2->priority < mp) {
					currjob = temp2;
					mp = temp2->priority;
				}
				temp2=temp2->next;
			}

			currjob->w_time = ct - currjob->a_time;
			printf("%d p%d ",ct, currjob->pn);
			state = 1;
			currjob->burst--;
		} else if(state == 0) {
			printf("%d (idle) ", ct);
		} else if(state==1) {
			currjob->burst--;
			if(currjob->burst==0) {
				state=0;
				currjob->ta_time = ct+1 - currjob->a_time;
				if(currjob==temp) {
					temp=temp->next;
				}
				removeProcess(currjob);
			}
		}
		ct++;
	}
	printf("%d\n",ct);

	printf("| Process | Turnaround time | Waiting time |\n");
	printf("+---------+-----------------+--------------+\n");
	for(i=0;i<n;i++) {
		printf("|p%8d|%17d|%14d|\n", p[i].pn,p[i].ta_time,p[i].w_time);
		sumta+=p[i].ta_time;
		sumw +=p[i].w_time;
	}
	printf("+------------------------------------------+\n");
	printf("Average waiting time: %f\nAverage turnaround time: %f\n",sumw/n, sumta/n);
}

main() {
	int i;

	printf("Enter number of processes: ");
	scanf("%d", &n);

	for(i=0;i<n;i++) {
		p[i].pn = i;
		printf("Enter arrival time for process p%d: ", i);
		scanf("%d",&p[i].a_time);
		printf("Enter CPU burst for process p%d: ", i);
		scanf("%d", &p[i].burst);
		p[i].next = NULL;
		printf("Enter priority for process p%d: ", i);
		scanf("%d", &p[i].priority);
	}

	buildProcessList(p,n);

	displayInformation();
}
