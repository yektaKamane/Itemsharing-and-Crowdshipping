
class Node {
  public:
    float x;
    float y;
};

class Trip {
  public:
    Node src;  
    Node dest;
};

void genetic_algorithm (Node *supplies, Node *requests, Trip *trips, int data_size, int number_of_set);
void create_initial_population (int population_size, int data_size, int ***population);
void print_population (int top, int intop, int ***population);
void calculate_fitness (float *fitness, int ***population, int population_size, int data_size, Node *supply, Node *req, Trip *trip);
float get_distance (float longitude, float latitude, float otherLongitude, float otherLatitude);
void print_fitness (float *fitness, int population_size);
void rank_select (float *fitness, int population_size, int ***population, int *selection_pool);
void roulette_wheel_select(float *fitness, int population_size, int ***population, int *selection_pool);
void one_point_crossover(int population_size, int data_size, int ***population, int *selection_pool);
void two_point_crossover(int population_size, int data_size, int ***population, int *selection_pool);
void small_crossover(int population_size, int data_size, int ***population, int *selection_pool, Node *supply, Node *req, Trip *trip);
void mutation(int population_size, int data_size, int ***population, Node *supply, Node *req, Trip *trip);
void write_results(int data_size, int ***population, int number_of_set, Node *supply, Node *req, Trip *trip);
float get_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index);
float get_max_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index);
void write_temp(float fit_value, int set_number, int data_size);