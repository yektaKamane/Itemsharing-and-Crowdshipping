
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

void genetic_algorithm(Node *supplies, Node *requests, Trip *trips, int data_size);
void create_initial_population(int population_size, int ***population);