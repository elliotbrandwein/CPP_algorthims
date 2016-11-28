#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>	
using namespace std;
int main(int argc, char** argv)
{
	class listNode
	{
	public:
		string data;
		listNode* next;
		listNode( string input)
		{
			data = input;
		}
		void free_node()
		{
			data = "";
			next = NULL;
		}
	};
	class linkedListStack
	{
	public:
		listNode* top;
		linkedListStack()
		{
			top = NULL;
		}
		string pop()
		{
			// this should never happen
			if(top == NULL) exit(0);

			if(top->next == NULL)
			{
				string s = top->data;
				top = NULL;
				return s;
			}
			listNode* temp = top;
			while(temp->next->next != NULL)
			{
				temp = temp->next;
			}
			string output = temp->next->data;
			temp->next->free_node();
			temp->next = NULL;
			return output;
		}
		void push(listNode* input)
		{
			if(top == NULL) top = input;
			else
			{
				listNode* temp = top;
				top = input;
				input->next = temp;
			}
			return;
		}
		int isEmpty()
		{
			if(top == NULL) return 1;
			return 0;
		}
		void printStack(ofstream &outFile)
		{
			if (top == NULL )return;
			listNode* temp = top;
			while(temp->next != NULL)
			{
				outFile<<temp->data<<",";
				temp = temp->next;
			}
			outFile<<temp->data<<endl;
			return;
		}
	};
	class linkedListQueue
	{
	public:
		listNode* head;
		listNode* tail;
		linkedListQueue()
		{
			listNode* dummy = new listNode("dummy");
			tail = dummy;
			head = dummy;
		}
		void addTail(listNode* newTail)
		{
			listNode* temp = head;
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = newTail;
			tail = newTail;
		}

		string deleteHead()
		{
			// for a 1 element queue we need to move tail
			if(head->next->next == NULL)
			{
				string s = head->next->data;
				head ->next = NULL;
				tail = head;
				return s;
			}

			string s  = head->next->data;
			head->next = head->next->next;
			return s;
		}

		int isEmpty()
		{
			if(head->next == NULL) return 1;
			return 0;
		}

		void printQueue(ofstream &outFile)
		{
			if (head->next == NULL )
			{
				outFile<<head->data<<endl;
				return;
			}
			listNode* temp = head->next;
			while(temp->next != NULL)
			{
				outFile<<temp->data<<",";
				temp = temp->next;
			}
			outFile<<temp->data<<endl;
			return;
		}
	};

	class RadixSort
	{
	public:
		linkedListQueue* hashTable[2][256];
		int currentTable;
		int currentDigit;
		int tableIndex;
		void addTail(listNode* node, int hashIndex)
		{
			hashTable[currentTable][hashIndex]->addTail(node);
			return;
		}
		char getVal(listNode* node)
		{
			string s = node->data;
			int index = s.length();
			index--;
			return (char) s[index-currentDigit];
		}
		int hashIndex(char val)
		{
			return (int) val;
		}
		RadixSort()
		{
			for(int i = 0; i<2 ;i++)
			{
				for(int c = 0; c<256;c++)
				{
					hashTable[i][c] = new linkedListQueue();
				}
			}
		}
		void printTable( ofstream &outFile)
		{
			int index = 0;
			while(index<256)
			{
				if( hashTable[currentTable][index]->isEmpty() == 0 )
				{
					outFile<<"Table ["<<currentTable<<"] ["<<index<<"]: ";
					hashTable[currentTable][index]->printQueue(outFile);
				}
				index++;
			}
			outFile<<endl;
		}
		listNode* padString(listNode* node, int longestStringLength)
		{
			if(node->data.length() == longestStringLength)return node;

			string s = node->data;
			while(s.length() < longestStringLength)

			{
				s= ' '+s;
			}
			node->data = s;
			return node;
		}

	};
	//step 0
	fstream inFile;
	ofstream outFile;
	inFile.open(argv[1]);
	outFile.open(argv[2]);
	string input;
	linkedListStack* stack = new linkedListStack();
	int longestStringLength = 0;
	// step 1 and 2
	while(inFile >> input)
	{
		if(input.length() > longestStringLength) 
			longestStringLength = input.length();

		listNode* newNode = new listNode(input);
		stack->push(newNode);
	}
	outFile<<"PRINTING STACK:"<<endl;
	//step 3
	stack->printStack(outFile);	
	//step 4
	RadixSort* buckets = new RadixSort();
	//step 5
	buckets->currentDigit = 0;
	buckets->currentTable = 0;
	//step 6 and 7
	char val;
	int hashIndex;
	
	while(stack->isEmpty() == 0)
	{
		listNode* temp = new listNode(stack->pop());
		temp = buckets->padString(temp, longestStringLength);
		val = buckets->getVal(temp);
		hashIndex = buckets->hashIndex(val);
		buckets->addTail(temp,hashIndex);
	}
	//step 8
	outFile<<endl<<"printing buckets: "<<"1"<<endl;
	buckets->printTable(outFile);
	//step 9
	buckets->currentDigit++;
	buckets->currentTable = 1;
	int prevTable = 0;
	//step 10
	while(buckets->currentDigit <= longestStringLength)
	{
		listNode* temp;
		for(int i = 0; i<256; i++)
		{
			while(!buckets->hashTable[prevTable][i]->isEmpty())
			{
				listNode* temp =  new listNode(buckets->hashTable[prevTable][i]->deleteHead());
				val = buckets->getVal(temp);
				hashIndex = buckets->hashIndex(val);
				buckets->addTail(temp,hashIndex);
			}	
		}
		buckets->currentDigit++;
		outFile<<"printing buckets: " <<buckets->currentDigit<<endl;
		buckets->printTable(outFile);
		if(buckets-> currentTable == 0)
		{
			buckets->currentTable = 1;
			prevTable = 0;
		}
		else if (buckets ->currentTable == 1)
		{
			buckets->currentTable = 0;
			prevTable = 1;
		}
 	}
	return 0;
}