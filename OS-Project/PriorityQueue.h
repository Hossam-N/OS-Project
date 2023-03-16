#pragma once



typedef short bool;
#define true 1
#define false 0
// LOW PRIORITY VALUE INDICATES A HIGH PRIORITY PROCESS
//======================================================

struct PriQueue
{
	struct Node *head;
};

// This returns a pointer to an empty priority queue
struct PriQueue *newPriQueue(int size)
{
	struct PriQueue *priQueue = (struct PriQueue *)malloc(sizeof(struct PriQueue)*size);
	priQueue->head = NULL;
	return priQueue;
}

bool pushInPriQueue(struct PriQueue *pQ, struct ProcessData process)
{
	if(!pQ)
		return false;

	struct Node *node = newNode(process);
	struct Node *head = pQ->head;
	if (!head || (node->process.priority < head->process.priority))
	{

		node->next = head;
		pQ->head = node;
	}
	else
	{
		while (head->next && head->next->process.priority <= node->process.priority)
		{
			head = head->next;
		}
		node->next = head->next;
		head->next = node;
	}
	return true;
}

struct Node *popFromPriQueue(struct PriQueue *pQ)
{
	if (!pQ->head)
		return NULL;
	struct Node *node = pQ->head;
	pQ->head = pQ->head->next;
	return node;
}

struct Node *peekFromPriQueue(struct PriQueue *pQ)
{
	if (!pQ->head)
		return NULL;
	return pQ->head;
}

void printPriQueue(struct PriQueue *pQ)
{
	struct Node *head = pQ->head;
	while (head)
	{
		printf("ProcessId: %d, Priority: %d, Arrival: %d\n", head->process.pid, head->process.priority, head->process.arrivalTime);
		head = head->next;
	}
	printf("----------------------------------- \n");
}

bool isPriQueueEmpty(struct PriQueue *pQ)
{
	return (pQ->head == NULL);
}


// //==============================================================================
// // HOW TO USE IT?
// //==================
// // 1 Create priQueue
// //==================
// // struct PriQueue *pQ = newPriQueue();
// //===================
// // 2 push in PriQueue
// //===================
// // pushInPriQueue(pQ, 1, 5);
// // pushInPriQueue(pQ, 2, 3);
// // pushInPriQueue(pQ, 3, 8);
// //===================
// // 3 Pop from priQueue
// //===================
// // struct Node *node = popFromPriQueue(pQ);
// // 3 print PriQueue
// //==================
// // printPriQueue(pQ);
// //=============================
// // 4 Check if priqueue is empty
// //=============================
// // printf("is priQueue emty: %d \n", isPriQueueEmpty(pQ));
// //==============================================================================
