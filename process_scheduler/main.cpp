#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
class graphNode
{
public:
	int jobId;
	graphNode* next = NULL;
	graphNode(){}
	graphNode(int id)
	{
		jobId = id;
	}
};

class hashNode
{
public:
	void push(graphNode* newNode)
	{
		if(stackTop == NULL)
		{
			stackTop = newNode;
			return;
		}
		else
		{
			graphNode* temp  = stackTop;
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = newNode;
			return;
		}
	}
	void remove_parent(int id)
	{
		if(stackTop == NULL)
			return;
		if(stackTop->jobId == id)
		{
			stackTop = stackTop->next;
			fatherCount--;
			return;
		}
		graphNode* temp = stackTop;
		while(temp->next != NULL)
		{
			if(temp->next->jobId == id)
			{
				temp->next = temp->next->next;
				fatherCount--;
				return;
			}
			temp =  temp->next;
		}
	}

	int jobTime = 0;
	int fatherCount = 0;
	int childCount = 0;
	graphNode* stackTop = NULL;
	hashNode(){}
	
};

graphNode* insert(graphNode* OPEN, graphNode* orphan, hashNode* graphHashTable)
{
	if(OPEN == NULL)
		return orphan;
	else
	{
		if(graphHashTable[OPEN->jobId].jobTime > graphHashTable[orphan->jobId].jobTime)
		{
			graphNode* temp = OPEN;
			OPEN = orphan;
			OPEN->next = temp;
			return OPEN;
		}
		else if(graphHashTable[OPEN->jobId].jobTime == graphHashTable[orphan->jobId].jobTime && graphHashTable[OPEN->jobId].childCount < graphHashTable[orphan->jobId].childCount)
		{
		graphNode* temp = OPEN;
		OPEN = orphan;
		OPEN->next = temp;
		return OPEN;
		}
		graphNode* temp = OPEN;
		while(temp->next != NULL)
		{
			if(graphHashTable[temp->next->jobId].jobTime > graphHashTable[orphan->jobId].jobTime)
			{
				graphNode* swap = temp->next;
				temp->next = orphan;
				orphan->next = swap;
				return OPEN;
			}
			else if(graphHashTable[temp->next->jobId].jobTime == graphHashTable[orphan->jobId].jobTime && graphHashTable[temp->next->jobId].childCount < graphHashTable[orphan->jobId].childCount)
			{
				graphNode* swap = temp->next;
				temp->next = orphan;
				orphan->next = swap;
				return OPEN;
			}
			temp = temp->next;
		}
		temp->next = orphan;
		return OPEN;
	}
}

void remove_kids (hashNode* graphHashTable, int doneJob, int numJobs)
{
	for(int i = 1; i<=numJobs; i++)
		graphHashTable[i].remove_parent(doneJob);
}

int main(int argc, char** argv)
{
	
	int numJobs;
	int numProcessors;
	int totaljobTimes;
	int Time = 0;
	int done_jobs = 0;
	int** processorSchedule; 
	hashNode* graphHashTable;
	graphNode* OPEN = NULL;
	fstream infile1, infile2;
	ofstream outfile;

	//step 0;
	infile1.open(argv[1]);
	infile2.open(argv[2]);
	outfile.open(argv[4]);
	// atoi converts char** from input to string, is found in cstdlib.
	numProcessors = atoi(argv[3]);
	infile1 >> numJobs;
	if(numProcessors > numJobs)
		numProcessors = numJobs;
	Time++;
	

	//step 1:
	graphHashTable = new hashNode[numJobs+1];

	//step 2 & 3: 
	int ni, nj;
	while(infile1 >> ni >> nj)
	{
		int job = ni, child = nj;
		graphNode* newNode = new graphNode(job);
		graphHashTable[child].push(newNode);
		graphHashTable[job].childCount++;
		graphHashTable[child].fatherCount++;
	}

	//step 4: 
	totaljobTimes = 0;

	//steps 5 & 6:
	int job, jobTime;

	// skip the first number of the second input, its a repeat of the first
	infile2 >> job;
	while(infile2 >> job >> jobTime)
	{
		graphHashTable[job].jobTime = jobTime;
		totaljobTimes += jobTime;
	}

	//step 7:
	int rows = numProcessors+1;
	int cols = totaljobTimes+1;
	processorSchedule = new int*[rows];
	for(int i = 0; i<rows; i++)
		processorSchedule[i] = new int[cols];
	processorSchedule[1][0] = 0;


	// this is where the actual making of the schedule begins a.k.a step 25
	while(done_jobs < numJobs)
	{
		//steps 8,9,10 & 11:
		job = 1;
		while(job <= numJobs)
		{
			if(graphHashTable[job].fatherCount == 0)
			{
				graphHashTable[job].fatherCount--;
				OPEN = insert(OPEN,new graphNode(job), graphHashTable);
			}
			job++;
		}

		//step 12:
		int processor = 1;
		int avail_pro,slot;
		//steps 13,14,15,16,17,18 & 19:
		while(OPEN !=NULL && processor <= numProcessors)
		{
			if(OPEN != NULL && processorSchedule[processor][0] <= 0)
			{
				avail_pro = processor;
				processorSchedule[avail_pro][0]++;
				job = OPEN->jobId;
				OPEN = OPEN->next;
				jobTime = graphHashTable[job].jobTime;
				slot = Time;
				while(slot < Time + jobTime)
				{
					processorSchedule[avail_pro][slot] = job;
					slot++;	
				}
			}
			processor++;
		}
		//step 20:
		Time++;

		//step 21:
		processor = 1;

		//step 22,23 & 24:
		while(processor <= numProcessors)
		{
			if(processorSchedule[processor][0] > 0 && processorSchedule[processor][Time] <= 0)
			{
				int doneJob = processorSchedule[processor][Time-1];
				graphHashTable[doneJob].fatherCount--;
				processorSchedule[processor][0] =0;
				remove_kids(graphHashTable,doneJob,numJobs);
				// this is for step 25
				done_jobs++;
			}
			processor++;
		}
	}

	// this part prints the graph
	outfile<<"TIME:         |";
	for(int i = 1; i<=totaljobTimes; i++)
	{	
		if(i<10)
			outfile<<" "<<i<<"|";
		else
			outfile<<i<<"|";
	}

	outfile<<endl;
	for(int i = 1; i<=128;i++)
		outfile<<"-";
	for(int i = 1; i<=numProcessors; i++)
	{
		if(i< 10)
			outfile<<endl<<"processor "<< i<< ":  |";
		else
			outfile<<endl<<"processor "<< i<< ": |";
		for(int j = 1; j<=totaljobTimes; j++)
		{
			if(processorSchedule[i][j] == 0)
				outfile<<" x|";
			else if(processorSchedule[i][j] != 0 && i<10  && processorSchedule[i][j]<10)
				outfile<<" "<<processorSchedule[i][j]<<"|";
			else if(processorSchedule[i][j] != 0 && i<10  && processorSchedule[i][j]>=10)
				outfile<<processorSchedule[i][j]<<"|";
			else
				outfile<<processorSchedule[i][j]<<"|";
			
		}
	}

	infile1.close();
	infile2.close();
	outfile.close();
	return 0;
}