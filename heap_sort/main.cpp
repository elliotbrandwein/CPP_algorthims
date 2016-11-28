	#include <iostream>
	#include <fstream>
	#include <string>
	#include <cstdlib>
	using namespace std;

	int main(int argc, char** argv)
	{
		// step 0
		class PQSort
		{
		private: 
			int* PQAry;
			int PQAry_length;
		public:

			PQSort(int length)
			{
				PQAry = new int[length];
				// this is the max amount of the length
				PQAry_length = length;
				// this is used to keep track of the current length
				PQAry[0] = 0;
			}
			~PQSort()
			{
				delete PQAry;
			}

			void buildPQAry(char** argv)
			{
				ifstream inFile;
				ofstream out1;
				inFile.open(argv[1]);
				out1.open(argv[2]);
				int data;
				while(inFile >>data)
				{
					this->Insert(data);
					this->printPQAry(out1);
				}
				out1<<"END OF BUILDING HEAP"<<endl<<endl;
				inFile.close();
				out1.close();
			}

			void Insert(int new_int)
			{
				if(this->PQAry_length == PQAry[0])
				{
					cout<<"out of bounds on heap";
					exit(0);
				}
				PQAry[(PQAry[0]+1)] = new_int;
				PQAry[0] = PQAry[0]+1;
				this->BubbleUp(PQAry[0]);
			}

			void swap (int child, int parent)
			{
				int temp = PQAry[child];
				PQAry[child]= PQAry[parent];
				PQAry[parent] = temp;
			}

			void BubbleUp(int spot)
			{
				int parent_spot = spot/2;
				while(PQAry[spot]<PQAry[parent_spot])
				{
					this->swap(spot, parent_spot);
					spot = parent_spot;
					parent_spot = spot/2;
					if(spot <=1) break;
				}
			}

			void deletePQAry(char** argv)
			{
				ofstream out1, out2;
				out1.open(argv[2], ios_base::app);
				out2.open(argv[3]);

				out1<<"START OF DELETING HEAP"<<endl;
				while(PQAry[0] > 0)
				{
					this->printPQAry(out1);
					this->Delete(out2);
					
				}

				out1.close();
				out2.close();
			}

			void Delete(ofstream &out2)
			{
				if(PQAry[0]==0)
				{
					cout<<"out of bounds error";
					exit(0);
				}

				out2<<PQAry[1]<<" ";

				// leaf int not needed, but here for human readability
				int leaf_location = PQAry[0];

				PQAry[1]= PQAry[leaf_location];
				leaf_location--;
				PQAry[0] = leaf_location;

				// if we deleted our last item
				if(PQAry[0]== 0) return;

				this->BubbleDown();
			}

			void BubbleDown()
			{
				int root = 1;
				int left = 2;
				int right = 3;
				int min, min_place;
				while(true)
				{
					if(left > PQAry[0]) return;
					if( PQAry[root] < PQAry[left] && PQAry[root] < PQAry[right]) return;
					min = PQAry[left];
					min_place = left;
					if(PQAry[right]<min)
					{
						min = PQAry[right];
						min_place = right;
					}
					if(min >= PQAry[root])
					{
						return;
					}
					this->swap(root,min_place);
					root = min_place;
					left = root*2;
					right = left+1;

				}
				
			}

			bool isPQAryFull()
			{
				if(PQAry[0]==PQAry_length) return true;
				return false;
			}

			bool isPQAryEmpty()
			{
				if(PQAry[0]==0) return true;
				return false;
			}

			void printPQAry(ofstream &out1)
			{
				int length = PQAry[0];
				if (length > 10)
				{
					length = 10;
				}
				for(int i = 1; i<=length;i++)
				{
					out1<<PQAry[i]<<" ";
				}
				out1<<endl;
				
			}
		};


		
			
		//step 1
		ifstream inFile;
		inFile.open(argv[1]);
		

		int count = 0;
		int temp;
		while(inFile >> temp)
		{
			count++;
		}
			
		PQSort heap = PQSort(count+1);
		inFile.close();

		//step 2
		heap.buildPQAry(argv);

		//step 3
		heap.deletePQAry(argv);
		
		
	return 0;
	}