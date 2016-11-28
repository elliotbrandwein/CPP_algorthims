#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv)
{
	ifstream inFile;
	static ofstream outFile;
	inFile.open(argv[1]);
	outFile.open(argv[2]);
	class listNode
	{
		friend class linkedList;
		
		int count = 0;
		listNode *next = NULL;
	public:
		string data = "";
		listNode(string input){
			data = input;
		};
		listNode(){
			data = "dummy";
		}

	};
	class linkedList
	{
	private:
		listNode *listHead = new listNode();
	public:
		
		listNode* findSpot(string data)
		{
			listNode *spot = listHead;
			listNode *nextNode;
			while(true)
			{
				
  				std::string str2 (data);
				if (spot->next != NULL && str2.compare(spot->next->data) > 0){
					spot = spot->next;
				}
				else{
					return spot;
				}
			}
		}
		void listInsert(listNode* spot, string data){
				listNode* newNode = new listNode(data);
				listNode *temp = spot->next;
				spot->next = newNode;
				newNode->next = temp;
		}
		void printList()
		{
			listNode *pointer = listHead;
			outFile<<"listHead ";
			while(pointer != NULL)
			{
				outFile<<"--> ";
				if (pointer -> data == "dummy"){
					outFile<<"(dummy,";
				}
				else{
					outFile<<"("<<pointer -> data<<",";
				}
				if(pointer ->next == NULL){
					outFile<<"null)"<<endl;
					return;
				}
				else{
					pointer = pointer->next;
					outFile<<pointer -> data<<")";
				}
			}
			outFile<<endl;
			
		}
	};
	
	string words;
	linkedList *list = new linkedList();
	listNode *spot;
	while(inFile >> words){
		outFile<<"Insert Data ("<<words<<"):"<<endl;
		spot = list->findSpot(words);
		list->listInsert(spot,words);
		list->printList();
	}
	delete list;
	outFile.close();
	inFile.close();
	return 0;
}