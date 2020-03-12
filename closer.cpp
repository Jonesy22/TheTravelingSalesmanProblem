#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <deque>
#include "math.h"
#include <vector>
#include <cstdlib>
#include <signal.h>
#include <string.h>

//#include <bits/stdc++.h> 

using namespace std;

class City 
{ 

	public: 
  
    int x;
    int y;
    int id;
    int pos;
    bool placed;

// Constructor that takes all members as input
City(int idin, int xin, int yin, int posin, bool placedin) : id(idin), x(xin), y(yin), pos(posin), placed(placedin) {}

// Copy constructor
City(City & source)
{
    x = source.x;
    y = source.y;
    pos = source.pos;
    id = source.id;
    placed = source.placed;
    //neighbor_list = source.neighbor_list;
}

// Destructor (no dynamic memory)
~City()
{
}

// Calculates the Euclidean distance to city_in


int get_x()
{
    return x;
}

int get_y()
{
    return y;
}

int get_id()
{
    return id;
}

int get_pos()
{
    return pos;
}





// Writes to file that is already open, takes ofstream object as parameter
int write_out(ofstream & write)
{
    if (write.is_open())
    {
        write << id << '\n';
        return 1;
    }
    
    return 0;
}




};

int dist(City* city_in, City* city)
{
    float x2 = city_in->x;
    float y2 = city_in->y;
    float x = city->x;
    float y = city->y;
    
    x2 -= x;
    y2 -= y;
    
    x2 *= x2;
    y2 *= y2;
    
    return floor(sqrt(x2 + y2) + 0.5);  // Rounds down from 0.0 to .499_, rounds up from 0.5 to .99_
}

int nearest_neighbor(int numCities,  deque <City*> originalList, deque <City*> solution, int** matrix, int* path);
int nearest_neighbor_basic(int start_index, int numCities, deque <City*> originalList, deque <City*> solution, int** matrix, int* path);
//int dist(City city_in, City City);
//int write_out(ofstream & write, int id);

void copy_city_deque(deque <City*> & source, deque <City*> & dest)
{
    int length = source.size();
    dest.clear();
    for (int i = 0; i < length; ++i)
    {
        dest.push_back(new City(*source[i]));
    }
}
int get_solution_distance(int numCities, deque <City*> solution, int** matrix)
{
    cout<<"g1"<<endl;
    int total_dist = 0;
    for (int i = 0; i < numCities - 1; ++i)
    {
        cout<<"g2 "<<i<<endl;
        cout<<total_dist<<endl;
        total_dist += matrix[solution[i]->id][solution[i+1]->id];
        //total_dist += dist(solution[i], solution[i+1]);
    }
    total_dist += matrix[solution[0]->id][solution[numCities-1]->id];
    cout<<"g3"<<endl;
    //total_dist += dist(solution[0], solution[numCities-1]);
    return total_dist;
}


int getDistance(int x[], int y[], int numCities, int i, int j, int *matrix[]) {
	if (i == j) {
		matrix[i][j] = 0;
		return matrix[i][j];
	}
	//If not, we use the rounding function on the sqrt function, to find the difference
	matrix[i][j] = round(sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2)));
	return matrix[i][j];
}

void getData(ifstream &inFile, string line, int numCities, int *identifier, int *x, int *y) {
	for (int i = 0; i < numCities; i++) {
		inFile >> identifier[i];
		inFile >> x[i];
		inFile >> y[i];
	}
}

//function prints the sorted arrays to a text file, "merge.txt"
void printtofile(char* name, int* path, int numCities, int totalDist){
    ofstream f;
	strcat(name, ".tour");
	cout << name;
    f.open(name);
    if(!f){ //if file cant be opened
        cout << "Cannot open file" << endl;
    }
    else{
		f << totalDist << endl;
		for(int i=0; i<numCities; i++){
			f<<path[i]<<endl;
		}
	}
    f.close();
}

int main(int argc, char* argv[]) {
	ifstream inFile;
	int numCities = 0;
	int p = 0;
	string line;
	if (argc != 2) {
		cout << "Incorrect number of arguments!" << endl;
		return 1;
	}
	inFile.open(argv[1]);
	while (getline(inFile, line)) {
		++numCities;
	}
	int x[numCities];
	int y[numCities];
	int identifier[numCities];
	inFile.clear();
	inFile.seekg(0);
	getData(inFile, line, numCities, identifier, x, y);
	int **matrix = new int*[numCities];
	for (int i = 0; i < numCities; i++) {
		matrix[i] = new int[numCities];
	}
	for (int i = 0; i < numCities; i++) {
		for (int j = 0; j < numCities; j++) {
			getDistance(x, y, numCities, i, j, matrix);
		}
	}


	    //New
    //=======Setup=======//
    int totalDist = 0;
    deque <City*> originalList;
	deque <City*> solution;
    //City originalList[numCities];
    //City solution[numCities];
    
    for(int i = 0; i< numCities; i++){
        /*
        originalList[i].x = x[i];
        originalList[i].y = y[i];
        originalList[i].id = identifier[i];
        originalList[i].placed = false;
        */
        originalList.push_back(new City(identifier[i], x[i], y[i], identifier[i], false));
        
    }
    //=======Setup=======//
    int path[numCities+1];
    //=======Call Stuff=======//
    totalDist = nearest_neighbor(numCities, originalList, solution, matrix, path);
    cout<<"Final: "<<totalDist<<endl;

    //=======Call Stuff=======//
    cout<<"path1 "<<path[0]<<endl;

    //=======Print to File=======//
    printtofile(argv[1], path, numCities, totalDist);
    //=======Print to File=======//
  
    return 0; 
}

int nearest_neighbor(int numCities,  deque <City*> originalList, deque <City*> solution, int** matrix, int* path)
{
    solution.clear();
    int total_dist = 0;
    int best_start_distance = 9999999;
    int last_run = 0;
    int pathT[numCities+1];

    // Run through entire 2-opt optimization for each City, write best index.
    for (int i = 0; i < numCities; ++i)
    {
       
        last_run = nearest_neighbor_basic(i, numCities, originalList, solution, matrix, pathT);

        
        
        if (last_run < best_start_distance)
        {
            best_start_distance = last_run;
            cout << "Writing solution " << best_start_distance << endl;
            cout<<"plz"<<solution.size()<<endl;
            //update path
            for(int i = 0; i<numCities+1; i++){
                path[i]=pathT[i];
            }
        }
    }

   
    cout<<"h1"<<endl;
    //total_dist = get_solution_distance(numCities, solution, matrix);
    cout<<"h2"<<endl;
    if (best_start_distance <= total_dist){
        cout<<"h3"<<endl;
        return best_start_distance;     // solution already written
        
    }else{
        cout<<"h4"<<endl;
        //cout << "Writing solution " << total_dist << endl;
        //write_solution(OUTPUT_FN);       //write current solution (midway through 2-opt)
        return best_start_distance;
    }

    
    
}

int nearest_neighbor_basic(int start_index, int numCities,  deque <City*> originalList, deque <City*> solution, int** matrix, int* pathT)
{
    int pathInd= 1;
    int cities_added = 0;
    int closest = 9999999;
    int total_dist = 0;
    int current_dist = 0;
    int closest_index = 0;
    int current_num = numCities;
    deque <City*> temp;
    copy_city_deque(originalList, temp);       // save original list

    solution.clear();
    solution.push_back(originalList[start_index]);     // move first City to solution
    originalList.erase(originalList.begin() + start_index);       // erase from originalList
    --current_num;        // cities remaining in originalList
    ++cities_added;       // number of cities in solution so far
    while(current_num != 0)             // loop until no cities remaining in originalList
    {
        closest = 9999999;  // reset closest to a large number so that comparison will work
        for (int i = 0; i < current_num; ++i)
        {
            
            current_dist = matrix[originalList[i]->id][solution[cities_added-1]->id];
            //current_dist = dist(originalList[i], solution[cities_added-1]);
            if (current_dist < closest)
            {
                closest_index = i;
                closest = current_dist;
            }
        }

        total_dist += closest;
        solution.push_back(originalList[closest_index]);
        pathT[pathInd] = solution.at(1)->id;
        pathInd++;
        originalList.erase(originalList.begin() + closest_index);

        --current_num;
        ++cities_added;
    }

    copy_city_deque(temp, originalList);        // restore original list
    int t = matrix[solution[0]->id][solution[cities_added-1]->id];
    //return total_dist + dist(solution[0], solution[cities_added-1]);
    pathT[0] = total_dist + t;
    return total_dist + t;
}
