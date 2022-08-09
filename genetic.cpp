#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include "genetic.h"
using namespace std;

void genetic_algorithm(Node *supplies, Node *requests, Trip *trips, int data_size, int number_of_set){

    int population_size = 100;
    int iteration_number = 500;

    int ***population = (int ***)malloc(population_size * sizeof(int **));
    double *fitness = (double *)malloc(population_size * sizeof(double));
    int *selection_pool = (int *)malloc(population_size * sizeof(int));
    // create initial population
    create_initial_population(population_size, data_size, population);
    // print_population(4, 4, population);

    for(int iteration=0; iteration<iteration_number; iteration++){
        // fitness calculation
        calculate_fitness(fitness, population, population_size, data_size, supplies, requests, trips);
        // print_population(4, 4, population);
        
        // selection
        // print fitness values to see which selection fits better
        // try out different selection methods

        // the index of selected populations
        rank_select(fitness, population_size, population, selection_pool);
        if (iteration == iteration_number-1){
            write_results(data_size, population, number_of_set, supplies, requests, trips);
            cout << data_size << " : " << fitness[0] << endl;
            break;
        }
        // roulette_wheel_select(fitness, population_size, population, selection_pool);
        // print_fitness(fitness, 20);
        // print_population(4, 4, population);
        // crossover
        // try out different crossover methods
        small_crossover(population_size, data_size, population, selection_pool, supplies, requests, trips);

        // mutation
        mutation(population_size, data_size, population, supplies, requests, trips);
        // print_fitness(fitness, 5);
        
    }
    // write_results(data_size, population, number_of_set, supplies, requests, trips);
    free(population);
    free(fitness);
    free(selection_pool);
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

double get_max_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index){
    double max_profit = 0.0;
    int best_type = 0;
    for (int i=0; i<3; i++){    
        population[pop_index][2][chrmsm_index] = i;
        double profit = get_profit(population, supply, req, trip, pop_index, chrmsm_index);
        if (profit > max_profit){
            max_profit = profit;
            best_type = i;
        }
    }
    population[pop_index][2][chrmsm_index] = best_type;
    return max_profit;
}

double get_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index){
    double profit = 0.0;
    double speed = 30.0;
    int delivery_type = population[pop_index][2][chrmsm_index];
    // get the coordinates of the supplier
    double s_x = supply[chrmsm_index].x;
    double s_y = supply[chrmsm_index].y;
    // get the coordinates of the request
    double r_x = req[population[pop_index][0][chrmsm_index]].x;
    double r_y = req[population[pop_index][0][chrmsm_index]].y;
    // get the coordinates of the trip
    double ts_x = trip[population[pop_index][1][chrmsm_index]].src.x;
    double ts_y = trip[population[pop_index][1][chrmsm_index]].src.y;
    double td_x = trip[population[pop_index][1][chrmsm_index]].dest.x;
    double td_y = trip[population[pop_index][1][chrmsm_index]].dest.y;

    if (delivery_type == 0){
    // self-sourcing
    // check feasibility
        double direct_trip_distance = get_distance(s_y, s_x, r_y, r_x);
        double direct_trip_duration = direct_trip_distance / speed;
        if (direct_trip_duration * 60.0 <= 10){
            profit = 10;
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
        if (detour_duration <= 0.2 * direct_trip_duration){
            profit = 15 - (30 * detour_duration);
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
        if ((detour_duration <= 0.2 * direct_trip_duration) && (demander_duration * 60.0 <= 10.0)){
            profit = 15 - (30 * detour_duration);
        }
    }
    return profit;
}


void calculate_fitness(double *fitness, int ***population, int population_size, int data_size, Node *supply, Node *req, Trip *trip){
    double speed = 30;
    for (int i=0; i<population_size; i++){
        double sum = 0;
        for (int j=0; j<data_size; j++){
            sum += get_profit(population, supply, req, trip, i, j);
        }
        fitness[i] = sum;
    }
}

void rank_select(double *fitness, int population_size, int ***population, int *selection_pool){
    int temp;
    int **temp_p;
    // sort the fitness array
    for (int i=0; i<population_size-1; i++){
        for (int j = 0; j < population_size - i - 1; j++){
            if (fitness[j] < fitness[j + 1]){
                // swap IDs in the fitness array
                temp = fitness[j];
                fitness[j] = fitness[j+1];
                fitness[j+1] = temp;

                // swap IDs in the population array
                temp_p = population[j];
                population[j] = population[j+1];
                population[j+1] = temp_p;
            }
        }
    }

    for (int i=0; i<population_size/2; i++){
        selection_pool[i] = i;
        selection_pool[i + population_size/2] = i;
    }
}

void roulette_wheel_select(double *fitness, int population_size, int ***population, int *selection_pool){
    float *abs_probility = (float *)malloc(population_size * sizeof(float));
    float *cum_probility = (float *)malloc(population_size * sizeof(float));
    double sum_of_fitness = 0;
    float sum_of_prob = 0;
    for (int i=0; i<population_size; i++){
        sum_of_fitness += fitness[i];
    }

    // cout << "sum of fitness : " << sum_of_fitness << endl;

    for (int i=0; i<population_size; i++) {
        abs_probility[i] = (float)fitness[i]/sum_of_fitness;
        // cout << abs_probility[i] << " , ";
    }
    // cout << "\n\n";

    for (int i=0; i<population_size; i++) {
        cum_probility[i] = sum_of_prob + abs_probility[i];
        sum_of_prob = cum_probility[i];
        // cout << cum_probility[i] << " , ";
    }
    // cout << "\n\n";

    for (int i=0; i<population_size; i++){
        float random = (float) rand()/RAND_MAX;
        int j=0;
        while(cum_probility[j]<random && j<population_size-1) j++;
        selection_pool[i] = j;
        // cout << "random : " << random << endl;
        // cout << "j : " << j << endl;
        // cout << "cum pro : " << cum_probility[j] << endl;
        // cout << "\n";
    }
    free(abs_probility);
    free(cum_probility);
}


void one_point_crossover(int population_size, int data_size, int ***population, int *selection_pool){
    int *seen = (int *)malloc(population_size * sizeof(int));
    for (int i=0; i<population_size; i++) seen[i] = -1;
    for (int i=0; i<population_size/2; i++){
        // randomly pick two chromosomes
        int random1 = (rand() % population_size-1) + 1;
        while(seen[random1] != -1) random1 = (rand() % population_size-1) + 1;
        seen[random1] = 1;
        int random2 = (rand() % population_size-1) + 1;
        while(seen[random2] != -1) random2 = (rand() % population_size-1) + 1;
        seen[random2] = 1;

        // cout << "random generations : " << selection_pool[random1] << " , " << selection_pool[random2] << endl;
        // randomly pick two numbers in range of the population size
        int rand_num1 = (rand() % data_size-1) + 1;
        int rand_num2 = (rand() % 3) + 1;

        // cout << "random cuts : " << rand_num1 << " , " << rand_num2 << endl;
        
        int population_index1 = selection_pool[random1];
        int population_index2 = selection_pool[random2];
        for (int i=0; i<rand_num2; i++){
            for (int j=0; j<rand_num1; j++){
                // swap everything before that cut
                int temp = population[population_index1][i][j];
                population[population_index1][i][j] = population[population_index2][i][j];
                population[population_index2][i][j] = temp;
            }
        }
    }
    free(seen);
}

void two_point_crossover(int population_size, int data_size, int ***population, int *selection_pool){
    int *seen = (int *)malloc(population_size * sizeof(int));
    for (int i=0; i<population_size; i++) seen[i] = -1;
    for (int i=0; i<population_size/2; i++){
        // randomly pick two chromosomes
        int random1 = (rand() % population_size-1) + 1;
        while(seen[random1] != -1) random1 = (rand() % population_size-1) + 1;
        seen[random1] = 1;
        int random2 = (rand() % population_size-1) + 1;
        while(seen[random2] != -1) random2 = (rand() % population_size-1) + 1;
        seen[random2] = 1;

        // cout << "random generations : " << selection_pool[random1] << " , " << selection_pool[random2] << endl;
        // randomly pick two numbers in range of the population size        
        int rand_num = (rand() % 3) + 1;
        int rand_num1 = (rand() % data_size-1) + 1;
        int rand_num2 = (rand() % data_size-1) + 1;
        int min = std::min(rand_num1, rand_num2);
        int max = std::max(rand_num1, rand_num2);
        // cout << "random cuts : " << rand_num1 << " , " << rand_num2 << endl;
        
        int population_index1 = selection_pool[random1];
        int population_index2 = selection_pool[random2];
        for (int i=0; i<rand_num; i++){
            for (int j=min; j<max; j++){
                // swap everything before that cut
                int temp = population[population_index1][i][j];
                population[population_index1][i][j] = population[population_index2][i][j];
                population[population_index2][i][j] = temp;
            }
        }
    }
    free(seen);
}

void small_crossover(int population_size, int data_size, int ***population, int *selection_pool, Node *supply, Node *req, Trip *trip){
    int *seen = (int *)malloc(population_size * sizeof(int));
    for (int i=0; i<population_size; i++) seen[i] = -1;
    for (int i=0; i<population_size/2; i++){
        // randomly pick two chromosomes
        int random1 = (rand() % population_size-1) + 1;
        while(seen[random1] != -1) random1 = (rand() % population_size-1) + 1;
        seen[random1] = 1;
        int random2 = (rand() % population_size-1) + 1;
        while(seen[random2] != -1) random2 = (rand() % population_size-1) + 1;
        seen[random2] = 1;

        // randomly pick two numbers in range of the population size        
        int rand_num1 = (rand() % data_size-1) + 1;
        int rand_num2 = (rand() % data_size-1) + 1;
        int rand_num3 = (rand() % data_size-1) + 1;

        int points[] = {rand_num1, rand_num2, rand_num3};
        // cout << "random cuts : " << rand_num1 << " , " << rand_num2 << endl;
        
        int population_index1 = selection_pool[random1];
        int population_index2 = selection_pool[random2];

        for (int i=0; i<2; i++){
            for (int point : points){
                int temp = population[population_index1][i][point];
                population[population_index1][i][point] = population[population_index2][i][point];
                population[population_index2][i][point] = temp;                
            }
        }
        for (int point : points){
            get_max_profit(population, supply, req, trip, population_index1, point);  
            get_max_profit(population, supply, req, trip, population_index2, point);  
        }
        // cout << "done" << endl;
    }
    free(seen);
}

void mutation(int population_size, int data_size, int ***population, Node *supply, Node *req, Trip *trip){
    // balances the occurrences of each id
    int *seen = (int *)malloc(data_size * sizeof(int));
    for (int i=0; i<population_size; i++){
        for (int j=0; j<2; j++){
            for (int n=0; n<data_size; n++) seen[n] = -1;
            for (int k=0; k<data_size; k++){
                int val = population[i][j][k];
                if (seen[val] == -1){
                    seen[val] = 1;
                }
                else{
                    int counter = val;
                    while(seen[counter] != -1) {
                        if (counter >= data_size) counter = 0;
                        else counter++;
                        // cout << counter << endl;
                    }
                    population[i][j][k] = counter;
                    seen[counter] = 1;

                    get_max_profit(population, supply, req, trip, i, k);  
                }
            }
        }
    }
    free(seen);
}

double get_distance(double longitude, double latitude, double otherLongitude, double otherLatitude){
    double pi = 3.14159;
    double d1 = latitude * (pi / 180.0);
    double num1 = longitude * (pi / 180.0);
    double d2 = otherLatitude * (pi / 180.0);
    double num2 = otherLongitude * (pi / 180.0) - num1;
    double d3 = pow(sin((d2 - d1) / 2.0), 2.0) + cos(d1) * cos(d2) * pow(sin(num2 / 2.0), 2.0);

    double res = 6376500.0 * (2.0 * atan2(sqrt(d3), sqrt(1.0 - d3)));
    // cout << res *0.6 / 1000 << endl;
    return (res * 0.6) / 1000 ;
    // return 0;
}

void write_results(int data_size, int ***population, int number_of_set, Node *supply, Node *req, Trip *trip){
    string MyText = "";
    string dir = "D:\\Project_Data\\Generated_Coordinates\\Sample" + std::to_string(number_of_set) + "\\NewResult" + std::to_string(data_size) + "\\assignment.txt";
    // cout << dir << endl;
    for (int i=0; i<data_size; i++){
        MyText += "sup id: " + std::to_string(i);
        MyText += "\nreq id: " + std::to_string(population[0][0][i]);
        MyText += "\ntrip id: " + std::to_string(population[0][1][i]);
        MyText += "\ndel type: " + std::to_string(population[0][2][i]);

        int delivery_type = population[0][2][i];
        // get the coordinates of the supplier
        double s_x = supply[i].x;
        double s_y = supply[i].y;
        // get the coordinates of the request
        double r_x = req[population[0][0][i]].x;
        double r_y = req[population[0][0][i]].y;
        // get the coordinates of the trip
        double ts_x = trip[population[0][1][i]].src.x;
        double ts_y = trip[population[0][1][i]].src.y;
        double td_x = trip[population[0][1][i]].dest.x;
        double td_y = trip[population[0][1][i]].dest.y;

        double speed = 30;
        if (delivery_type == 0){
            // self-sourcing
            // check feasibility
            double direct_trip_distance = get_distance(s_y, s_x, r_y, r_x);
            double direct_trip_duration = direct_trip_distance / speed;
            if (direct_trip_duration * 60.0 <= 10){
                MyText += "\ndel result: ssrc";
                MyText += "\nprofit : 10";
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
            if (detour_duration <= 0.2 * direct_trip_duration){
                // cout << 15 - (30 * detour_duration) << endl;
                MyText += "\ndel result: home";
                MyText += "\nprofit : " + std::to_string( 15 - (30 * detour_duration));
    
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
            if ((detour_duration <= 0.2 * direct_trip_duration) && (demander_duration * 60.0 <= 10)){
                MyText += "\ndel result: neigh";
                MyText += "\nprofit : " + std::to_string( 15 - (30 * detour_duration));
            }
        }

        MyText += "\n------\n";
    }
    // cout << MyText << endl;
    ofstream myFile;
    myFile.open(dir);
    myFile << MyText;
    myFile.close();
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

void print_fitness(double *fitness, int population_size){
    for (int i=0; i<population_size; i++){
        cout << fitness[i] << " , ";
    }
    cout << "\n";
}