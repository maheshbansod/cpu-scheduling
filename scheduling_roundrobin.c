#include <stdio.h>
#include <stdlib.h>
/** this program is under construction as it was found to have defects after rigourous testing by a meticulous and extremely enthusiastic tester**/

//CPU scheduling - round robin
//Processes are numbered from p0,..,pn by default

typedef struct proc {
	int pn;
	int a_time;
	int burst;

	int na_time; //new a_time useful for preemptive: reset whenever it enters ready queue
	
	int ta_time;
	int w_time;

	struct proc *next;
} Process;
Process *ps=NULL;
Process p[100];
int n,tq; //no. of processes and time quantum

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
	//make it circular
	temp = ps;
	while(temp->next != NULL) temp=temp->next;
	temp->next = ps;
}

void removeProcess(Process *p) {
	Process *temp = ps;
	
	if(p == ps) {
		if(ps->next == ps) ps=NULL; //if there's only one element left
		else
			ps=ps->next;
		return;
	}

	while(temp->next != ps && temp->next != p) {
		temp=temp->next;
	}
	if(temp->next==ps) return;

	temp->next = p->next;
}

void displayInformation() {
	Process *temp = ps, *temp2;
	int i, mb=0;
	int ct = 0; //current time
	int pt = 0; //time for which a process holds CPU continuously
	int state = 0; //0- idle, 1- process running
	float sumta=0,sumw=0;
	
	
	printf("Gantt chart: \n");
	while(ps != NULL) {
		if(temp->na_time <= ct) {

			if(pt==tq && temp->next->na_time <= ct ) { //if one time quantum passed and next process arrived. i.e. preemption
				pt=0;
				temp->na_time = ct;
				temp = temp->next;//select next process
			}

			if(pt == 0) { //start a process
				temp->w_time += ct - temp->na_time;
				printf("%d p%d \n",ct, temp->pn);
				state=1;
			}
			
		}
		
		if(state == 0) {
			printf("%d (idle) ", ct);
			ct=temp->na_time-1;
		} else if(state==1) {
			temp->burst--;
			pt++;

			if(temp->burst==0) {
				state=0;
				temp->ta_time = ct+1 - temp->a_time;
				temp2=temp;
				temp=temp->next;
				pt=0;
				removeProcess(temp2);
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

	printf("Enter time quantum: ");
	scanf("%d", &tq);

	for(i=0;i<n;i++) {
		p[i].pn = i;
		p[i].ta_time = p[i].w_time = 0;
		printf("Enter arrival time for process p%d: ", i);
		scanf("%d",&p[i].a_time);
		p[i].na_time = p[i].a_time;
		printf("Enter CPU burst for process p%d: ", i);
		scanf("%d", &p[i].burst);
		p[i].next = NULL;
	}

	buildProcessList(p,n);

	displayInformation();
}
