
class Node {
  public:
    double x;
    double y;
};

class Trip {
  public:
    Node src;  
    Node dest;
};

void genetic_algorithm (Node *supplies, Node *requests, Trip *trips, int data_size, int number_of_set);
void create_initial_population (int population_size, int data_size, int ***population);
void print_population (int top, int intop, int ***population);
void calculate_fitness (double *fitness, int ***population, int population_size, int data_size, Node *supply, Node *req, Trip *trip);
double get_distance (double longitude, double latitude, double otherLongitude, double otherLatitude);
void print_fitness (double *fitness, int population_size);
void rank_select (double *fitness, int population_size, int ***population, int *selection_pool);
void roulette_wheel_select(double *fitness, int population_size, int ***population, int *selection_pool);
void one_point_crossover(int population_size, int data_size, int ***population, int *selection_pool);
void two_point_crossover(int population_size, int data_size, int ***population, int *selection_pool);
void small_crossover(int population_size, int data_size, int ***population, int *selection_pool, Node *supply, Node *req, Trip *trip);
void mutation(int population_size, int data_size, int ***population, Node *supply, Node *req, Trip *trip);
void write_results(int data_size, int ***population, int number_of_set, Node *supply, Node *req, Trip *trip);
double get_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index);
double get_max_profit(int ***population, Node *supply, Node *req, Trip *trip, int pop_index, int chrmsm_index);