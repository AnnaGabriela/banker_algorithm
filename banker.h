#define TRUE 1
#define FALSE 0
#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define MAX_REQUESTS_FILE "./max_requests.txt"

/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* temporary states of data structures */
int available_temp[NUMBER_OF_RESOURCES];
int allocation_temp[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need_temp[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

void get_available_amount(int argc, char **argv);
void get_maximum_requests();
void get_amount_needed();
void get_command();
int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);
void print_matrix(int matrix[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]);
void print_vector(int vector[NUMBER_OF_RESOURCES]);
void output_data_structures();