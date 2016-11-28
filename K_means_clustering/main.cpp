#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;
double dist(int x1, int y1, int x2, int y2)
{
	int a = (x2-x1)*(x2-x1);
	int b = (y2-y1)*(y2-y1);
	return sqrt((a+b));
}
int main(int argc, char** argv)
{
	class Point
	{	
	public:	
		int x_coord;
		int y_coord;
		int cluster_ID;
		double distance;
		Point(int x, int y)
		{
			x_coord = x;
			y_coord = y;
		}
		void printPoint(ofstream &outFile1)
		{
			outFile1<<x_coord<<" "<< y_coord<< " "<<cluster_ID<<endl;
		}
	};
	class K_mean
	{
	public:
		int k;
		int numPts;
		int numRow;
		int numCol;
		int** imageArray;
		int* clusters_avg_counter;
		Point** pointSet;
		struct xyCoord
		{
			int x_coord;
			int y_coord;
		};
		xyCoord *Kcentroids;
		K_mean(int k_given, int row_total, int col_total, int point_num)
		{
			k = k_given;
			numRow = row_total;
			numCol = col_total;
			numPts = point_num;
			Kcentroids = new xyCoord[k];
			imageArray = new int*[row_total];
			clusters_avg_counter = new int[k_given];
			for(int i = 0; i<row_total; i++)
			{
				imageArray[i] = new int[col_total];
			}
			pointSet = new Point*[point_num];
		} 
		void loadPointSet(fstream &inFile)
		{
			int counter = 0;
			int row, col;
			while(inFile >> row >>col)
			{
				pointSet[counter] = new Point(row, col);
				counter++;
			}
		}
		void assignLabel()
		{
			int rando = rand();
			for(int i = 0; i<numPts;i++)
				pointSet[i]->cluster_ID = ((i+rando)%4)+1;
		}
		void mapPoint2Image()
		{
			for(int i = 0; i< numPts; i++)
				imageArray[pointSet[i]->x_coord][pointSet[i]->y_coord] = pointSet[i]->cluster_ID;
		}
		void displayImage(ofstream &outFile2)
		{
			for(int i = 0; i<numRow; i++)
			{
				for(int j = 0; j<numCol;j++)
				{
					if(imageArray[i][j]==0)outFile2<<" ";
					else outFile2<<imageArray[i][j];
				}
				outFile2<<endl;
			}
		}
		void printAllPoints(ofstream &outFile1)
		{
			outFile1<<k<<endl;
			outFile1<<numPts<<endl;
			outFile1<<numRow<<" "<< numCol<<endl;
			for(int i = 0; i<numPts;i++)
				pointSet[i]->printPoint(outFile1);
		}
		void computeCentroids()
		{
			//reset the counter from last run;
			for(int i = 0; i< k; i++)
				clusters_avg_counter[i] = 0;
			for(int i = 0; i<numPts;i++)
			{
				if(pointSet[i]->cluster_ID == 1)
				{
					Kcentroids[0].x_coord += pointSet[i]->x_coord;
					Kcentroids[0].y_coord += pointSet[i]->y_coord;
					clusters_avg_counter[0] += 1;
				}
				else if(pointSet[i]->cluster_ID == 2)
				{
					Kcentroids[1].x_coord += pointSet[i]->x_coord;
					Kcentroids[1].y_coord += pointSet[i]->y_coord;
					clusters_avg_counter[1]+= 1;
				}
				else if(pointSet[i]->cluster_ID == 3)
				{
					Kcentroids[2].x_coord += pointSet[i]->x_coord;
					Kcentroids[2].y_coord += pointSet[i]->y_coord;
					clusters_avg_counter[2]+= 1;
				}
				else if(pointSet[i]->cluster_ID == 4)
				{
					Kcentroids[3].x_coord += pointSet[i]->x_coord;
					Kcentroids[3].y_coord += pointSet[i]->y_coord;
					clusters_avg_counter[3]+= 1;
				}
			}
			for(int i = 0; i<k ;i++)
			{
				Kcentroids[i].x_coord = Kcentroids[i].x_coord/clusters_avg_counter[i]; 
				Kcentroids[i].y_coord = Kcentroids[i].y_coord/clusters_avg_counter[i];
			}
		}
		void minDistCalc()
		{
			double temp = 0;
			for(int i = 0; i<numPts;i++)
			{
				pointSet[i]->cluster_ID = 1;
				pointSet[i]->distance = dist(pointSet[i]->x_coord,pointSet[i]->y_coord,Kcentroids[0].x_coord,Kcentroids[0].y_coord);
				for(int j = 1; j<k; j++)
				{
					temp = dist(pointSet[i]->x_coord,pointSet[i]->y_coord,Kcentroids[j].x_coord,Kcentroids[j].y_coord);
					if(temp<pointSet[i]->distance)
					{
						pointSet[i]->cluster_ID = j+1;
						pointSet[i]->distance = temp;
					}
				}
			}
		}
		bool minDistSetter()
		{
			int attempt = 1;
			bool changeLabel = false;
			double temp = 0;
			for(int i = 0; i<numPts;i++)
			{
				for(int j = 0; j<k; j += 1)
				{
					if( (j+1) != pointSet[i]->cluster_ID)
					{
						temp = dist(pointSet[i]->x_coord, pointSet[i]->y_coord, Kcentroids[j].x_coord, Kcentroids[j].y_coord);
						if(temp<pointSet[i]->distance)
						{
							pointSet[i]->cluster_ID = j+1;
							pointSet[i]->distance = temp;
							changeLabel = true;
						}
					}
					
					
				}
			}
			return changeLabel;
		}
	};
	//step 0
	fstream inFile;
	ofstream outFile1, outFile2;
	inFile.open(argv[1]);
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);
	int k,point_num,row_total,col_total,img_num = 0;
	inFile >> k;
	inFile >> point_num;
	inFile >> row_total >> col_total;
	K_mean clusters(k,row_total,col_total,point_num);
	//step 1, load the points from the file into the cluster class variable pointSet
	clusters.loadPointSet(inFile);
	//step 2
	clusters.assignLabel();
	//step 3 
	clusters.mapPoint2Image();
	//step 4
	outFile2<<"image number: "<<++img_num<<endl;
	clusters.displayImage(outFile2);
	//step 5
	clusters.computeCentroids();
	//step 6-7
	clusters.minDistCalc();
	//step 8 (3-7 repeated)
	while(true)
	{
	clusters.mapPoint2Image();

	outFile2<<"image number: "<<++img_num<<endl;
	clusters.displayImage(outFile2);

	clusters.computeCentroids();

	// the minDist method is different then the first time
	if(!clusters.minDistSetter()) break;
	}
	//step 9
	clusters.printAllPoints(outFile1);
	return 0;
}