#include <iostream>
#include <cstdlib>
#include <cmath>
#include "genetic.h"
using namespace std;

void genetic_algorithm(Node *supplies, Node *requests, Trip *trips, int data_size){

    int population_size = 2 * data_size;
    int iteration_number = 100;

    int ***population = (int ***)malloc(population_size * sizeof(int **));
    int *fitness = (int *)malloc(population_size * sizeof(int));

    // create initial population
    create_initial_population(population_size, data_size, population);
    // print_population(20, 10, population);

    for(int iteration=0; iteration<iteration_number; iteration++){

        // fitness calculation
        calculate_fitness(fitness, population, population_size, data_size, supplies, requests, trips);
        // print_fitness(fitness, population_size);

        // selection
        // print fitness values to see which selection fits better
        // try out different selection methods

        // crossover

        // mutation
    }
    free(population);
}



void create_initial_population(int population_size, int data_size, int ***population){
    // memory allocation
    for (int i=0; i<population_size; i++){
        int **temp = (int **)malloc(3 * sizeof(int *));
        for (int j=0; j<3; j++){
            int *temp2 = (int *)malloc(data_size * sizeof(int));
            temp[j] = temp2;
        }
        population[i] = temp;
    }
    
    for (int i=0; i<population_size; i++){
        // loop over memebrs of a population
        for (int j=0; j<2; j++){
            // loop over Req and Trips separately

            // filling the array seen with -1 values
            int *seen = (int *)malloc(data_size * sizeof(int));                
            for (int k=0; k<data_size; k++){
                seen[k] = -1;
            }

            for (int k=0; k<data_size; k++){
                // filling each chromosome with unique random numbers 
                // in the range of the dataset
                int random = (rand() % data_size-1) + 1;
                while(seen[random] != -1){
                    random = (rand() % data_size-1) + 1;
                }
                population[i][j][k] = random;
                seen[random] = 1;
            }
            free(seen);
        }

        int j=2;
        for (int k=0; k<data_size; k++){
            int random = (rand() % 3);
            population[i][j][k] = random;
        }
    }

    // if (data_size == 10){
    //     cout << population[19][1][9] << endl;
    // }
    
}

void calculate_fitness(int *fitness, int ***population, int population_size, int data_size, Node *supply, Node *req, Trip *trip){
    double speed = 30;
    for (int i=0; i<population_size; i++){
        double sum = 0;
        for (int j=0; j<data_size; j++){
            // get the delivery type 
            int delivery_type = population[i][2][j];
            // get the coordinates of the supplier
            double s_x = supply[j].x;
            double s_y = supply[j].y;
            // get the coordinates of the request
            double r_x = req[population[i][0][j]].x;
            double r_y = req[population[i][0][j]].y;
            // get the coordinates of the trip
            double ts_x = trip[population[i][1][j]].src.x;
            double ts_y = trip[population[i][1][j]].src.y;
            double td_x = trip[population[i][1][j]].dest.x;
            double td_y = trip[population[i][1][j]].dest.y;

            // cout << s_y << "," << s_x << ";" << r_y << "," << r_x << endl;
            // cout << get_distance(s_y, s_x, r_y, r_x) << endl;
            // cout << "\n\n";

            if (delivery_type == 0){
                // self-sourcing
                // check feasibility
                double direct_trip_distance = get_distance(s_y, s_x, r_y, r_x);
                double direct_trip_duration = direct_trip_distance / speed;
                if (direct_trip_duration * 60.0 < 10){
                    sum += 10;
                }
                
            }
            else if (delivery_type == 1){
                // home del
                // check feasibility
                double direct_trip_distance = get_distance(ts_y, ts_x, td_y, td_x);
                double direct_trip_duration = direct_trip_distance / speed;
                double detour_distance = get_distance(ts_y, ts_x, s_y, s_x)
                                   + get_distance(s_y, s_x, r_y, r_x)
                                   + get_distance(r_y, r_x, td_y, td_x)
                                   - direct_trip_distance;
                double detour_duration = detour_distance / speed;
                if (detour_duration < 0.2 * direct_trip_duration){
                    sum += 15 - (30 * detour_duration);
                }
                
            }
            else if (delivery_type == 2){
                // neighborhood del
                // check feasibility
                double direct_trip_distance = get_distance(ts_y, ts_x, td_y, td_x);
                double direct_trip_duration = direct_trip_distance / speed;
                double detour_distance = get_distance(ts_y, ts_x, s_y, s_x)
                                   + get_distance(s_y, s_x, td_y, td_x)
                                   - direct_trip_distance;
                double detour_duration = detour_distance / speed;
                double demander_distance = get_distance(r_y, r_x, td_y, td_x);
                double demander_duration = demander_distance / speed;
                if ((detour_duration < 0.2 * direct_trip_duration) && (demander_duration * 60.0 < 10)){
                    sum += 15 - (30 * detour_duration);
                }
            }
        }
        fitness[i] = sum;
    }
}

double get_distance(double longitude, double latitude, double otherLongitude, double otherLatitude){
    double M_PI = 3.14159;
    double d1 = latitude * (M_PI / 180.0);
    double num1 = longitude * (M_PI / 180.0);
    double d2 = otherLatitude * (M_PI / 180.0);
    double num2 = otherLongitude * (M_PI / 180.0) - num1;
    double d3 = pow(sin((d2 - d1) / 2.0), 2.0) + cos(d1) * cos(d2) * pow(sin(num2 / 2.0), 2.0);

    double res = 6376500.0 * (2.0 * atan2(sqrt(d3), sqrt(1.0 - d3)));
    return (res + 0.1) / 1000;
}

void print_population(int top, int intop, int ***population){
    for (int i=0; i<top; i++){
        cout << "generation " << i << " : " << endl;
        for (int j=0; j<3; j++){
            // cout << j << endl;
            for (int k=0; k<intop; k++){
                cout << population[i][j][k] << " ,";
            }
            cout << "\n";
        }
        cout << "\n\n";
    }
}

void print_fitness(int *fitness, int population_size){
    for (int i=0; i<population_size; i++){
        cout << fitness[i] << " , ";
    }
    cout << "\n";
}