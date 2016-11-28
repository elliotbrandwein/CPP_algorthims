#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main(int argc, char** argv)
{
	class graphNode
	{
	public:
		int nodeID = 0;
		graphNode* next = NULL;
		graphNode(int id)
		{
			nodeID = id;
		}
		void insert(graphNode* newNode)
		{
			if(next == NULL)
			{
				next = newNode;
				return;
			}
			graphNode* temp =next;
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next  = newNode; 
		}
	};
	class edgeNode
	{
	public:
		int Ni;
		int Nj;
		int edgeCost;
		edgeNode* next;
		void freeEdgeNode()
		{
			next = NULL;
		}
		void printEdgeNode(ofstream &outFile)
		{
			outFile<<Ni<<" "<<Nj<<" "<<edgeCost<<endl;
		}
		edgeNode(int ni, int nj, int eCost)
		{
			Ni = ni;
			Nj = nj;
			edgeCost = eCost;
		}
		edgeNode* findSpot(edgeNode* newNode)
		{
			edgeNode *pre = this;
			edgeNode *post = this->next;
			while(post != NULL)
			{
				if(newNode->edgeCost < post->edgeCost)
				{
					return pre;
				}
				else
				{
					pre = post;
					post = post->next;
				}
			}
			return pre;
		}
		void append(edgeNode* newNode)
		{
			edgeNode* temp = this;
			while(temp->next != NULL)
				temp = temp->next;

			temp->next = newNode; 
		}
		void insertSort(edgeNode* newNode)
		{
			edgeNode* temp;
			edgeNode* spot = findSpot(newNode);
			temp = spot->next;
			spot->next = newNode;
			newNode->next = temp;
		}
	};

	class PrimeMST
	{
	public:
		int N = 0;
		int totalCost = 0;
		int* graphNodeIDarray;
		edgeNode* edgeList = new edgeNode(0,0,0);
		edgeNode* MSTofG   = new edgeNode(0,0,0);
		graphNode* setA = new graphNode(0);
		graphNode* setB = new graphNode(0);
		PrimeMST(fstream &inFile)
		{
			inFile >> N;
			graphNodeIDarray = new int[N+1];
			int ni, nj,cost;
			while(inFile >>ni >>nj >>cost)
			{
				graphNodeIDarray[ni]++;
				graphNodeIDarray[nj]++;
				edgeList->insertSort(new edgeNode(ni,nj,cost));
			}
		}
		void printSetsAB(ofstream &outFile)
		{
			outFile<<endl<<"SET A:"<<endl;
			graphNode* temp = setA;
			outFile<<temp->next->nodeID;
			temp = temp->next;
			while(temp->next != NULL)
			{
				temp = temp->next;
				outFile<<"->"<<temp->nodeID;
			}
			outFile<<endl<<"SET B:"<<endl;
			if(setB->next == NULL)
				return;
			temp = setB;
			outFile<<temp->nodeID;
			while(temp->next->next != NULL)
			{
				temp = temp->next;
				outFile<<"->"<<temp->nodeID;
			}
		}
		void printMSTofG(ofstream &outFile)
		{
			outFile<<endl<<"MST of G:"<<endl;
			edgeNode* temp = MSTofG->next;
			while(temp != NULL)
			{
				temp->printEdgeNode(outFile);
				temp = temp->next;
			}

		}
		void finalPrint(ofstream &outFile)
		{
			outFile<<endl<<"A MST of the input graph: "<<endl;
			edgeNode* temp = MSTofG->next;
			while(temp != NULL)
			{
				temp->printEdgeNode(outFile);
				temp = temp->next;
			}
			outFile<<"Total Cost is: "<<totalCost;
		}
		void moveNodeToSetA(edgeNode* edge)
		{
			int nodeId;
			graphNode* temp = setB;
			while(edge->Ni != temp->nodeID && edge->Nj != temp->nodeID)
			{
				temp = temp->next;
			}
			nodeId = temp->nodeID;
			setA->insert(temp);
			deleteFromSetb(nodeId);
			temp->next = NULL;

		}
		void deleteFromSetb(int id)
		{
			if(setB->nodeID == id)
			{
				setB = setB->next;
				return;
			}

			graphNode* temp1 = setB;
			graphNode* temp2;
			while(temp1->next->nodeID != id)
			{
				temp1 = temp1->next;
			}
			temp1->next= temp1->next->next;
		}
		void insertMSTofG(edgeNode* newEdge)
		{
			totalCost+= newEdge->edgeCost;
			MSTofG->append(newEdge);
		}
		edgeNode* getEdgeNode()
		{	
			edgeNode* temp = edgeList->next;
			edgeNode* output;
			while(temp->next != NULL)
			{
				if(isBadEdge (temp->next))
					temp = temp->next;
				else
				{

					output = temp->next;
					temp->next= output->next;
					output->freeEdgeNode();
					return output;
				}

			}
			output = temp->next;
			temp->next = output->next;
			output->freeEdgeNode();
			return output;
		}
		void printGraphNodeID(ofstream &outFile)
		{
			outFile<<"graph node Id array:"<<endl;
			for(int i = 1; i<=N ;i++)
			{
				outFile<<"node "<<i<<": "<<graphNodeIDarray[i]<<endl;
			}
		}
		void debugPrint(ofstream &outFile)
		{
			edgeNode* temp = edgeList->next;
			outFile<<endl<<"EDGE LIST:"<<endl;
			for(int i = 0; i<10;i++)
			{
				temp->printEdgeNode(outFile);
				temp = temp->next;
			}
		}
		int findFirstNonZero()
		{
			int id;
			for(id = 1; id<N;id++)
			{
				if(graphNodeIDarray[id] != 0) return id;
			}
		}
		void addToSetA(graphNode* newNode)
		{
			graphNode* temp = setA;
			while(temp->next != NULL)
			{
				temp = temp->next;
			}
			temp->next = newNode;
		}
		void insertRestIntoSetB()
		{
			int in_set_a = findFirstNonZero();
			for(int i = 1 ;i<=N;i++)
			{
				if(i != in_set_a)
				{
					graphNode* newNode = new graphNode(i);
					insertIntoSetB(newNode);
				}
			}

		}
		void insertIntoSetB(graphNode* newNode)
		{
			graphNode* temp = setB;
			setB = newNode;
			setB->next = temp;
			
		}
		bool isBadEdge(edgeNode* edge)
		{
			int tempNi = edge->Ni;
			int tempNj = edge->Nj;
			bool ni_in_set = false;
			bool nj_in_set = false;
			graphNode* temp = setA->next;
			while(temp != NULL)
			{
				if(temp->nodeID == tempNj)
					ni_in_set = true;
				if(temp->nodeID == tempNi)
					nj_in_set = true;
				if(ni_in_set && nj_in_set)
					return true;
				temp = temp->next;
			}
			ni_in_set = false;
			nj_in_set = false;
			temp = setB;
			while(temp->next != NULL)
			{
				if(temp->nodeID == tempNj)
					ni_in_set = true;
				if(temp->nodeID == tempNi)
					nj_in_set = true;
				if(ni_in_set && nj_in_set)
					return true;
				temp = temp->next;
			}
			return false;
		}
	};
	//step 0
	fstream inFile;
	ofstream outFile1, outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	//step 1.1, 1.2, 1.3, 2
	PrimeMST* Prim = new PrimeMST(inFile);
	//step 2
	Prim->printGraphNodeID(outFile2);
	//step 3
	Prim->debugPrint(outFile2);
	//step 4
	graphNode* firstSetA = new graphNode(Prim->findFirstNonZero());
	Prim->addToSetA(firstSetA);
	//step 5, 6
	Prim->insertRestIntoSetB();

	// step for me:
	Prim->printSetsAB(outFile2);
	Prim->printMSTofG(outFile2);

	//step 7,8,9,10
	edgeNode* newEdge;
	while(Prim->setB->next != NULL)
	{
		// step 7
		newEdge = Prim->getEdgeNode();
		// //step 8
		Prim->insertMSTofG(newEdge);

		Prim->moveNodeToSetA(newEdge);
		// //step 9
		Prim->printSetsAB(outFile2);
		Prim->printMSTofG(outFile2);
	}
	//step 11
	Prim->finalPrint(outFile1);

	inFile.close();
	outFile1.close();
	outFile2.close();

	return 0;
}