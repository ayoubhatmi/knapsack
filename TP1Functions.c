#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>

int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
			b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}



// Structure for representing items
struct Item {
    int weight;
    int value;
};

// Function to compare items based on their utility (value/weight ratio)
int compare(const void *a, const void *b) {
    double ratio_a = ((struct Item *)a)->value / ((struct Item *)a)->weight;
    double ratio_b = ((struct Item *)b)->value / ((struct Item *)b)->weight;
    if (ratio_a > ratio_b) return -1;
    else if (ratio_a < ratio_b) return 1;
    else return 0;
}

// Function implementing the Knapsack - Linear Relaxation algorithm
double knapsackLinearRelaxation(struct Item items[], int n, double capacity) {
    // Sort items by decreasing utility (value/weight ratio)
    qsort(items, n, sizeof(struct Item), compare);

    double totalValue = 0.0;
    double remainingCapacity = capacity;

    // Iterate over sorted items
    for (int j = 0; j < n; j++) {
        if (remainingCapacity <= 0)
            break; // Knapsack is full, exit loop

        // Calculate fraction of item that can be added
        double fraction = (remainingCapacity < items[j].weight) ? remainingCapacity / items[j].weight : 1.0;

        // Update remaining capacity
        remainingCapacity -= fraction * items[j].weight;

        // Update total value
        totalValue += fraction * items[j].value;
    }

    return totalValue;
}



// Function implementing the greedy algorithm for the knapsack problem
int knapsackGreedy(struct Item items[], int n, int capacity, int solution[]) {
    // Sort items by decreasing utility (value/weight ratio)
    qsort(items, n, sizeof(struct Item), compare);

    // Initialize remaining capacity
    int remainingCapacity = capacity;

    // Initialize solution vector
    for (int i = 0; i < n; i++)
        solution[i] = 0;

    // Iterate over sorted items
    for (int j = 0; j < n; j++) {
        // If knapsack is full, exit loop
        if (remainingCapacity <= 0)
            break;

        // If remaining capacity can accommodate the entire item, add it
        if (remainingCapacity >= items[j].weight) {
            solution[j] = 1;
            remainingCapacity -= items[j].weight;
        }
    }

    return 0;
}





int KP_greedy(dataSet* dsptr)
{
	int rval = 0;
    

  // Create an array of struct Item from the dataset
    struct Item items[dsptr->n];
    for (int i = 0; i < dsptr->n; i++) {
        items[i].weight = dsptr->a[i];
        items[i].value = dsptr->c[i];
    }

    // Initialize solution vector
    int solution[dsptr->n];

    // Call the greedy knapsack algorithm
    knapsackGreedy(items, dsptr->n, dsptr->b, solution);

    // Print the solution
    printf("Greedy Solution vector: ");
    for (int i = 0; i < dsptr->n; i++)
        printf("%d ", solution[i]);
    printf("\n");



	return rval;
}



int KP_LP(dataSet* dsptr)
{
	int rval = 0;

    // Define the Knapsack capacity
    int capacity = dsptr->b;

    // Create an array of items from dataset
    struct Item items[dsptr->n];
    for (int i = 0; i < dsptr->n; i++) {
        items[i].weight = dsptr->a[i];
        items[i].value = dsptr->c[i];
    }

    // Call the Knapsack Linear Relaxation algorithm
    double maxValue = knapsackLinearRelaxation(items, dsptr->n, capacity);
    printf("Maximum value that can be obtained using Linear Relaxation: %.2f\n", maxValue);

	return rval;
}

