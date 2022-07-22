#include <iostream>
#include "genetic.h"
using namespace std;

void genetic_algorithm(Node *supplies, Node *requests, Trip *trips, int data_size){

    int population_size = data_size;
    int iteration_number = 100;

    int ***population = (int ***)malloc(population_size * sizeof(int **));

    // create initial population
    // create_initial_population(population_size, population);

    // for(int iteration=0; iteration<iteration_number; iteration++){

    //     // fitness calculation

    //     // selection

    //     // crossover

    //     // mutation
    // }
    free(population);
}

void create_initial_population(int population_size, int ***population){

    for (int i=0; i<population_size; i++){
            int **temp = (int **)malloc(population_size * sizeof(int *));
            for (int j=0; j<population_size; j++){
                int *temp2 = (int *)malloc(population_size * sizeof(int));
                temp[j] = temp2;
            }
            population[i] = temp;
        }
}