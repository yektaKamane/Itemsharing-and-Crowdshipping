#include <iostream>
#include "genetic.h"
using namespace std;

void genetic_algorithm(Node *supplies, Node *requests, Trip *trips, int data_size){

    int population_size = 2 * data_size;
    int iteration_number = 100;

    int ***population = (int ***)malloc(population_size * sizeof(int **));

    // create initial population
    create_initial_population(population_size, data_size, population);

    // for(int iteration=0; iteration<iteration_number; iteration++){

    //     // fitness calculation

    //     // selection

    //     // crossover

    //     // mutation
    // }
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
            // loop over req and trips separately
        }
    }

    // if (data_size == 10){
    //     cout << population[19][1][9] << endl;
    // }
    

}