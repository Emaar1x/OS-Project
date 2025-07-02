#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3
#define TIME_QUANTUM 2
Figure 2: Deadlock Example

typedef struct {
int pid, arrival, burst, remaining, priority;
bool completed;
} Process;

typedef struct {
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
} ResourceGraph;

Process processes[MAX_PROCESSES];
int n;
ResourceGraph rg;

void input_processes() {
printf("Enter number of processes (max %d): ", MAX_PROCESSES);
scanf("%d", &n);
for (int i = 0; i < n; i++) {
processes[i].pid = i;
printf("Process %d - Arrival Time: ", i);
scanf("%d", &processes[i].arrival);
printf("Process %d - Burst Time: ", i);
scanf("%d", &processes[i].burst);
printf("Process %d - Priority (lower is higher): ", i);
scanf("%d", &processes[i].priority);
processes[i].remaining = processes[i].burst;
processes[i].completed = false;
}
}

void display_processes() {
printf("\nPID\tArrival\tBurst\tRemain\tPriority\n");
for (int i = 0; i < n; i++) {

printf("%d\t%d\t%d\t%d\t%d\n", processes[i].pid, processes[i].arrival,
processes[i].burst, processes[i].remaining, processes[i].priority);
}
}

void hybrid_scheduler() {
int time = 0, completed = 0;
printf("\n--- Hybrid CPU Scheduler ---\n");
while (completed < n) {
int selected = -1, min_priority = 999;
for (int i = 0; i < n; i++) {
if (!processes[i].completed && processes[i].arrival <= time) {
if (processes[i].priority < min_priority) {
min_priority = processes[i].priority;
selected = i;
}
}
}
if (selected == -1) {
time++;
continue;
}
int exec_time = (processes[selected].remaining < TIME_QUANTUM)
? processes[selected].remaining : TIME_QUANTUM;
printf("Time %d: P%d running for %d units\n", time, selected, exec_time);
processes[selected].remaining -= exec_time;
time += exec_time;
if (processes[selected].remaining == 0) {
processes[selected].completed = true;
completed++;
printf("P%d completed.\n", selected);
}
}
}

bool detect_deadlock() {
bool finish[n];
int work[MAX_RESOURCES];
for (int i = 0; i < n; i++) finish[i] = false;
for (int i = 0; i < MAX_RESOURCES; i++) work[i] = rg.available[i];
for (int k = 0; k < n; k++) {
for (int i = 0; i < n; i++) {
bool can_finish = true;
if (!finish[i]) {
for (int j = 0; j < MAX_RESOURCES; j++) {
if (rg.max[i][j] - rg.allocation[i][j] > work[j]) {
can_finish = false;
break;
}
}
if (can_finish) {
for (int j = 0; j < MAX_RESOURCES; j++)
work[j] += rg.allocation[i][j];
finish[i] = true;
}
}
}
}
for (int i = 0; i < n; i++) {
if (!finish[i]) return true;
}
return false;
}

void simulate_deadlock() {
printf("\n--- Enter Resource Allocation Details ---\n");
for (int i = 0; i < n; i++) {
printf("Process %d\n", i);

for (int j = 0; j < MAX_RESOURCES; j++) {
printf(" Allocation of Resource %d: ", j);
scanf("%d", &rg.allocation[i][j]);
}
}

for (int i = 0; i < n; i++) {
printf("Process %d\n", i);
for (int j = 0; j < MAX_RESOURCES; j++) {
printf(" Maximum need of Resource %d: ", j);
scanf("%d", &rg.max[i][j]);
}
}

for (int j = 0; j < MAX_RESOURCES; j++) {
printf("Available units of Resource %d: ", j);
scanf("%d", &rg.available[j]);
}

if (detect_deadlock())
printf("Deadlock detected!\n");
else
printf("No deadlock detected.\n");
}

int main() {
int choice;
do {
printf("\n--- OS Simulation Menu ---\n");
printf("1. Enter Processes\n");
printf("2. Display Processes\n");
printf("3. Run Hybrid CPU Scheduler\n");
printf("4. Simulate Deadlock Detection (Interactive)\n");
printf("0. Exit\n");

printf("Enter choice: ");
scanf("%d", &choice);
switch (choice) {
case 1: input_processes(); break;
case 2: display_processes(); break;
case 3: hybrid_scheduler(); break;
case 4: simulate_deadlock(); break;
case 0: break;
default: printf("Invalid choice\n");
}
} while (choice != 0);
return 0;
}