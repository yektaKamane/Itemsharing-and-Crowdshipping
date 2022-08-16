#include <iostream>
#include <string>
#include <fstream>
#include <cstring> 
#include <sstream>
#include <cmath>
#include <time.h>
#include <algorithm>
#include "genetic.h"

using namespace std;

void read_data_node(int index, int ctrl, int data_size, Node *ptr){
    int coordinates_size = 2;
    string myText;
    string dir = "D:\\Project_Data\\Generated_Coordinates\\Sample" + std::to_string(index);
    if (ctrl == 0){
        dir += "\\supplies.txt";
    }
    else if (ctrl == 1){
        dir += "\\requests.txt";
    }
    int counter = 0;
    ifstream MyReadFile(dir);
    while( getline(MyReadFile, myText) && counter < data_size){  
        // in crowdshipper file
        std::replace(myText.begin(), myText.end(), ',', ' ');            
        string arr[coordinates_size];
        int i = 0;
        stringstream ssin(myText);
        while (ssin.good() && i < coordinates_size){
            ssin >> arr[i];
            ++i;
        }
        float x = stof(arr[0]);
        float y = stof(arr[1]);
        Node node;
        node.x = x;
        node.y = y;
        ptr[counter] = node;
        counter++;
    }
    // Close the file
    MyReadFile.close();    
}

void read_data_trip(int index, int data_size, Trip *ptr){
    int coordinates_size = 4;
    string myText;
    string dir = "D:\\Project_Data\\Generated_Coordinates\\Sample" + std::to_string(index) + "\\crowdshipper.txt";
    ifstream MyReadFile(dir);
    int counter = 0;
    while( getline(MyReadFile, myText) && counter<data_size){  
        // in crowdshipper file
        std::replace(myText.begin(), myText.end(), ',', ' ');            
        string arr[coordinates_size];
        int i = 0;
        stringstream ssin(myText);
        while (ssin.good() && i < coordinates_size){
            ssin >> arr[i];
            ++i;
        }
        float x1 = stof(arr[0]);
        float y1 = stof(arr[1]);
        float x2 = stof(arr[2]);
        float y2 = stof(arr[3]);
        Trip trip;
        Node node1;
        Node node2;
        node1.x = x1;
        node1.y = y1; 
        node2.x = x2;
        node2.y = y2; 
        trip.src = node1;
        trip.dest = node2;
        ptr[counter] = trip;
        counter++;
    }
    MyReadFile.close();
}

float average_travel_time(Trip *trips, int size){
    float sum = 0.0;
    float speed = 50.0;
    for (int i=0; i<size; i++){
        float time = get_distance(trips[i].src.y, trips[i].src.x, trips[i].dest.y, trips[i].dest.x) / speed * 60.0;
        sum += time;
        // if (time < 10.0 ) cout << i+1 << " : " << time << endl;
    }
    float average = (float)sum/size;
    cout << "average : " << average << endl;

    sum = 0.0;
    for (int i=0; i<size; i++){
        float time = get_distance(trips[i].src.y, trips[i].src.x, trips[i].dest.y, trips[i].dest.x) / speed * 60.0;
        sum += pow(time - average, 2);
    }
    float std = sqrt((float)sum/size);
    cout << "std : " << std << endl;
    return average;
}

int main(){

    std::cout << "begin" << std::endl;
    srand(time(0));
    // int num_of_sets = 1;
    int parts[] = { 10, 25, 50, 75, 100, 150, 200 };

    int num_of_sets = 1;
    // int parts[] = {200};

    for (int i=0; i<num_of_sets; i++){
        for (int part : parts){
            // std::cout << "parts" << std::endl;
            Node *supplies = (Node *)malloc(part * sizeof(Node));
            Node *requests = (Node *)malloc(part * sizeof(Node));
            Trip *trips    = (Trip *)malloc(part * sizeof(Trip));
            // read supplies
            read_data_node(i+1, 0, part, supplies);
            // read requests
            read_data_node(i+1, 1, part, requests);
            // read crowdshippers
            read_data_trip(i+1, part, trips);

            // average_travel_time(trips, part);
            genetic_algorithm(supplies, requests, trips, part, i+1);
            // free the memory allocated
            free(supplies);
            free(requests);
            free(trips);
        }
    }

    cout << "end" << endl;
    return 0;
}