/*
 * Nathan Owen, ncowen@email.wm.end, 930860382
 *
 * This bank simulation implements two major functions; a function that each
 * teller thread uses in order to listen for more customers and then service them in an
 * appropriate amount of time, and a customer generation function that handles populating
 * the queue as the simulation progresses. When a customer is added, any available teller thread
 * (this will be random as is expected witht threads) will take the next customer, and when all the
 * tellers are occupied with customers (with varying lengths of help times) the queue grows in length.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "banksim.h"



//Global Variables
struct BankInfo bankInfo;
pthread_mutex_t mut;

int main(int argc, char *argv[]){

	// Variable Definitions
	pthread_t tellerThreads[100];
	int numTellers;
	bankInfo.totalServiceTime = 0;
	bankInfo.queue = malloc(100000*sizeof(int));
	memset(bankInfo.queue, 0, sizeof(int));

	//Stand-ins for actually recieving arguments
	bankInfo.maxTime = atof(argv[1]);//.2; // How long the program will run in minutes
	bankInfo.customerGenerationChance =  atof(argv[2]);//0.5;
	numTellers = atoi(argv[3]);//10;
	bankInfo.standardServiceTime =  atof(argv[4]);
	bankInfo.updateInterval = atof(argv[5]);//.2; //How frequently the program will update

	//Initialize all of our teller threads
	int x = 0;
	for (x; x < numTellers ; x ++)
		pthread_create(&tellerThreads[x], NULL, serviceCustomer,NULL);


	//Start Generating customers
	pthread_t generatingThread;
	pthread_create(&generatingThread, NULL, generateCustomers, NULL);


	//Make sure threads are terminated
	int y =0;
	for (y; y < numTellers ; y++)
		pthread_join(tellerThreads[y], NULL);
	pthread_join(generatingThread, NULL);


	printf("Simulation Finished\n");
	printf("Customers Served: %d\n", bankInfo.customersServiced);
	printf("Average Customer Wait Time: %.2f minutes\n", ( bankInfo.totalServiceTime/ bankInfo.customersServiced) / 60.0 );

	free(bankInfo.queue);

	return 0;

}


/*
 * This function is enacted by each of the teller threads. Its procedure is to 
 * sit in a while loop until a patron is available to be serviced, and it uses mutex
 * locking to ensure that only one thread takes a customer at a time.  When a customer is
 * serviced, the time for that service is determined and added to the total for later averaging.
 */
void *serviceCustomer (){

	bankInfo.customersServiced = 0;

	while (!bankInfo.endSimulation){ //simulation is ended when queue is empty

		//allow only one thread to check for a customer at a time
		pthread_mutex_lock(&mut);

		//all items int the queue are initially zero. When a customer is added to the
		//queue, their ID takes place of the zero. 
		if (bankInfo.queue[bankInfo.customersServiced] > 0){

			bankInfo.customersServiced += 1;
			//printf("\tTeller is servicing customer %d\n", bankInfo.customersServiced);

			//Work the given time plus or minus thirty
			int serviceTime = abs(( (bankInfo.standardServiceTime * 60) - 30) +  (rand() % 60));
			bankInfo.totalServiceTime += serviceTime;

			pthread_mutex_unlock(&mut); 

			sleep(serviceTime);

		}
		else{
			pthread_mutex_unlock(&mut); 
			continue;
		}
	}

}

/*
 * This function has a continuous while loop, where each loop is exactly one second long. During
 * each loop, there is a chance (set by the user) that a customer will be generated. In addition,
 * periodically the while loop will coincide with the update interval and will subsequently print
 * information to the screen about the progress of the simulation. The loop terminates at the time 
 * the user specifies.
 */
void *generateCustomers (){

	int updateInterval = (int) (bankInfo.updateInterval * 60);
	int customerNumber = 1;
	int secondsElapsed = 0;
	int updates = 0;
	int queueLength = 10000;

	//Seed the random number generator
	srand (time(NULL));

	//Execute while loop once a second until termination, and randomly generate
	//customers as it goes.
	while (secondsElapsed < bankInfo.maxTime * 60)	{

		if ((secondsElapsed % updateInterval) == 0){
			printf("Time: %-6dQueue Length: %d\n", updates, customerNumber - bankInfo.customersServiced - 1);
			updates += 1;
		}

		sleep(1);
		//printf("Waited %d seconds\n", secondsElapsed);
		secondsElapsed += 1;

		//Generate a customer according to the provided chance per second
		if ( ((double) rand() / RAND_MAX) <= bankInfo.customerGenerationChance){


			if (queueLength > customerNumber){

				//printf("\tCustomer %d Queued\n", customerNumber);		
				bankInfo.queue[customerNumber - 1] = customerNumber; 
				customerNumber += 1;

			}

			else{ //realloc the queue

				int *temp = realloc(bankInfo.queue, 1000*sizeof(int));
				if ( temp != NULL ) //realloc was successful
				{
					bankInfo.queue = temp;
				}
				else //there was an error
				{
					free(bankInfo.queue);
					perror("Error allocating memory!\n");
				}
				queueLength += 1000;

			}

		}	

	}
	
	bankInfo.endSimulation = 1;
}
