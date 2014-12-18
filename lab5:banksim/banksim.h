#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

//Function Definitions
void *serviceCustomer();
void *generateCustomers ();

struct BankInfo {
	int *queue;
	int endSimulation;
	int customersServiced;
	float maxTime;
	float customerGenerationChance;
	float standardServiceTime;
	float totalServiceTime;
	float updateInterval;
};