#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "banker.h"

int main (int argc, char **argv) {
    get_available_amount(argc, argv);
    get_maximum_requests();
    get_initial_allocation();
    get_amount_needed();
    get_command();
   return 0;
}

void get_available_amount (int argc, char ** argv) {
    if (argc == 1) {
        printf("Error. You must specify the resources amount.");
    } else {
     for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
         available[i] = atoi(argv[i+1]);
     }
    }
}

void get_maximum_requests() {
    FILE *fp;
    int resource_amount, counter = 0, costumer = 0;
    char* filename = MAX_REQUESTS_FILE;
 
    fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Could not open file %s", filename);
    }

    while (fscanf(fp, " %d", &resource_amount) == 1) {
        if (counter == NUMBER_OF_RESOURCES) {
            costumer++;
            counter = 0;
        }
        maximum[costumer][counter] = resource_amount;
        counter++;
    }
    fclose(fp);
}

void get_initial_allocation() {
    FILE *fp;
    int resource_amount, counter = 0, costumer = 0;
    char* filename = ALLOCATION_FILE;
 
    fp = fopen(filename, "r");

    if (fp == NULL){
        printf("Could not open file %s", filename);
    }

    while (fscanf(fp, " %d", &resource_amount) == 1) {
        if (counter == NUMBER_OF_RESOURCES) {
            costumer++;
            counter = 0;
        }
        allocation[costumer][counter] = resource_amount;
        counter++;
    }
    fclose(fp);
}

void get_amount_needed() {
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

void print_matrix(int matrix[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]) {
    printf("    A   B   C   D  \n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("%d | ", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d | ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_vector(int vector[NUMBER_OF_RESOURCES]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d | ", vector[i]);
    }
    printf("\n");
}

void output_data_structures() {
    printf("Available amount of each resource: \n");
    print_vector(available);
    printf("\nMaximum demand of each customer: \n");
    print_matrix(maximum);
    printf("\nAmount currently allocated to each customer: \n");
    print_matrix(allocation);
    printf("\nRemaining need of each customer: \n");
    print_matrix(need);
}

void get_command() {
    while (TRUE) {
        char *input = malloc(20 * sizeof(char));
        char *args, *command;
        int customer, i = 0;
        int resources[NUMBER_OF_RESOURCES];

        printf("Type the command: \n");
        fgets (input, 20, stdin);

        // Get the command
        args = strtok(input, " ");
        input[strcspn(input, "\n")] = 0; 
        command = args;
    
        if (!strcmp(command, "*")) {
            output_data_structures();
        } else {
            // Get the customer
            args = strtok(NULL, " ");
            customer = atoi(args);
            args = strtok(NULL, " ");
            
            // Get the resources
            while (i < NUMBER_OF_RESOURCES) {
                resources[i] = atoi(args);
                args = strtok(NULL, " ");
                i++;
            }

            if (!strcmp(command, "RQ")) {
                printf("* REQUEST FROM CUSTOMER %d *\n", customer);
                request_resources(customer, resources);
            } else if (!strcmp(command, "RL")) {
                printf("* RELEASE FROM CUSTOMER %d *\n", customer);
                release_resources(customer, resources);
            } else {
                printf("Invalid command \n");
            }
        }
        free(input);
    }
}

bool check_safety() {
    bool finish[NUMBER_OF_CUSTOMERS] = { false };
    int work[NUMBER_OF_RESOURCES];
    bool is_safe = true;

    memcpy(work, available_temp, NUMBER_OF_RESOURCES*sizeof(int));

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        if (finish[i] == false) {
            is_safe = false;
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                if (need_temp[i][j] <= work[i]) {
                    work[i] = work[i] + allocation_temp[i][j];
                    finish[i] = true;
                    is_safe = true;
                }
            }
        }
    }
    return is_safe;
}

void save_current_state() {    
    memcpy(available_temp, available, NUMBER_OF_RESOURCES*sizeof(int));
    memcpy(allocation_temp, allocation, NUMBER_OF_CUSTOMERS*NUMBER_OF_RESOURCES*sizeof(int));
    memcpy(need_temp, need, NUMBER_OF_CUSTOMERS*NUMBER_OF_RESOURCES*sizeof(int));
}

void simulate_allocation(int customer, int resource, int request) {
    available_temp[resource] -= request;
    allocation_temp[customer][resource] += request;
    need_temp[customer][resource] -= request;
}

void allocate_resource(int customer, int resource, int request) {
    available[resource] -= request;
    allocation[customer][resource] += request;
    need[customer][resource] -= request;
}

int request_resources(int customer, int request[]) {
    bool is_safe = true;

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        // Check maximum allowed
        if (request[i] <= need[customer][i]) {
            // Check currently available
            if (request[i] <= available[i]) {
                // Simulate allocation
                save_current_state();
                simulate_allocation(customer, i, request[i]);
                if (!check_safety()) {
                    is_safe = false;
                    break;
                }
                allocate_resource(customer, i, request[i]);
            } else {
                printf("Requested %d and there is %d available \n", request[i], available[i]);
                is_safe = false;
                break;
            }
        } else {
            printf("Requested %d and the amount needed is %d \n", request[i], need[customer][i]);
            is_safe = false;
            break;
        }
    }

    if (is_safe) {
        printf("System is safe! Allocated successfully\n");
        return 0;
    }

    printf("System is unsafe!\n");
    return -1;
}

void release_resources(int customer, int release[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] += release[i];
        allocation[customer][i] -= release[i];
        need[customer][i] += release[i];
    }
    printf("Resources successfully released\n");
}