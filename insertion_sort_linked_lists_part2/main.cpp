#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv)
{
	// step 0
	class listNode
	{
	private: 
		friend class linkedList;
		listNode *next = NULL;

	public:
		int count = 0;
		string data = "";
		listNode(string input)
		{
			data = input;
		};
		listNode()
		{
			data = "dummy";
		}

	};


	class linkedList
	{
	private:
		
	public:
		listNode *listHead = new listNode();

		void printSortedList(ofstream &outFile, listNode* textListHead)
		{

			listNode *pointer = textListHead->next;
			while(pointer != NULL)
			{	
				outFile<<pointer -> data<<"  "<<pointer-> count<<endl;
				
				if(pointer ->next == NULL)
				{
					return;
				}
				else
				{
					pointer = pointer->next;
				}
			}
		}

		listNode* findSpot(listNode* head, string data)
		{
			listNode *spot = head;
			listNode *nextNode;
			string str (data);

			while(true)
			{
				if (spot->next != NULL && str.compare(spot->next->data) > 0)
				{
					spot = spot->next;
				}
				else if (spot->next != NULL && str.compare(spot->next->data) == 0)
				{
					 // not in spec exactly but this should be here 
                        spot->next->count++;
                        return NULL;
				}
				else
				{
					return spot; 
				}
			}
				
		}

		void listInsert(listNode* spot, string data)
		{
				listNode* newNode = new listNode(data);
				listNode *temp = spot->next;
				spot->next = newNode;
				newNode->next = temp;
		}

		void debugPrint(ofstream &outFile, listNode* list)
		{
			outFile<<endl;
			listNode *pointer = list;
			outFile<<"listHead ";
			while(pointer != NULL)
			{
				outFile<<"--> ";
				if (pointer -> data == "dummy")
				{
					outFile<<"(dummy,";
				}
				else
				{
					outFile<<"("<<pointer -> data<<",";
				}
				if(pointer ->next == NULL)
				{
					outFile<<"null)"<<endl;
					return;
				}
				else
				{
					pointer = pointer->next;
					outFile<<pointer -> data<<")";
				}
			}
		}
		bool isCommonWord(listNode* wordList, string data)
		{
			//skip the first node because its a dummy
			listNode *temp = wordList->next;
			string str (data);
			while(temp != NULL){
            	if (str.compare(temp->data) == 0)
            	{
                	return true;
            	}
                temp = temp->next;
            }
            return false;
		}
				
	};
	//step 1
	ifstream inFile1,inFile2;
	ofstream outFile1, outFile2;
	inFile1.open(argv[1]);
	outFile1.open(argv[3]);
	outFile2.open(argv[4]);

	//step 2
	linkedList *CwordlistHead = new linkedList();
	string commonWord;
	listNode *spot;

	// //step 3 and step 7
	while(inFile1 >> commonWord)
	{
		outFile1<<endl<<"Insert Data ("<<commonWord<<"):";
		//step 4
		spot = CwordlistHead->findSpot(CwordlistHead->listHead, commonWord);

		//step 5.1
		if (spot != NULL)
		{
			//step 5.2
			CwordlistHead->listInsert(spot, commonWord);
		}

	// 	//step 6
		CwordlistHead->debugPrint(outFile1, CwordlistHead->listHead);
	}

	// //step 8
	inFile2.open(argv[2]);

	// //step 9 
	int insertionCnt = 0;

	// //step10
	linkedList *textListHead = new linkedList();

	outFile1<<endl;
	string textWord;
	while(inFile2 >>textWord)
	{
		//step 12 
		if( textListHead->isCommonWord(CwordlistHead->listHead, textWord) == false)
		{
			//step 12.1
			spot = textListHead->findSpot(textListHead->listHead, textWord);

			//step 12.2
			if(spot != NULL)
			{
				textListHead->listInsert(spot, textWord);
				insertionCnt++;
			}
		}
		if(insertionCnt>=5)
		{
			outFile1<<endl<<"Insert Data ("<<textWord<<"):";
			textListHead->debugPrint(outFile1, CwordlistHead->listHead);
			insertionCnt = 0;
		}
	}
	textListHead->printSortedList(outFile2,textListHead->listHead);

	delete textListHead; 
	delete CwordlistHead;
	inFile1.close();
	inFile2.close();
	outFile1.close();
	outFile2.close();
return 0;
}