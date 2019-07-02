/*
 ============================================================================
 Name        : 	sbrent.c				Swarm Brent
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Pollard-Rho Brent build in C

mpicc -o "sb" sbrent.c brent.c -lgmp
mpirun -np 64 sb
 ============================================================================
*/
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "gmp.h"

#define BASE 10

int prbrent(mpz_t factor, const mpz_t n, mpz_t start);

int
main()
{	/* Start MPI ************************************************************/
	MPI_Init(NULL, NULL);				    // start up MPI
	int cores;					    // number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &cores);		    // get # of processes
	int rank; 					    // rank this process
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);		    // get process rank

	/* Assign Data **********************************************************/
	mpz_t RANK;	mpz_init_set_ui (RANK, rank);	    // set to a Bignumber
	mpz_t CORES;	mpz_init_set_ui (CORES, cores);	    // set to a Bignumber
	mpz_t TWO;	mpz_init_set_ui(TWO, 2);	    // set to a Bignumber
	mpz_t factor;	mpz_init_set_ui(factor, 1);	    // set to a Bignumber
	mpz_t seed;	mpz_init_set_ui(seed, 8);	    // set to a Bignumber
	mpz_t n;	mpz_init_set_ui(n, 0);		    // set to a Bignumber
	int limit = 230;
	char buff[limit];
	int tag = 0;    				    // tag for messages
	int source = 0;					    // rank of sender
	MPI_Status status;   				    // status for received
	char* rtn;					    // fgets return status

	/* Input data & distribute it *******************************************/
	if(rank == 0) 				    	    // the control node
	{   printf("\n rank %d input\n", rank);		    // prompt input
	    rtn = fgets(buff, limit, stdin);		    // alternate input
	    int dest;
	    for(dest = 1; dest < cores; dest++)		    // send to each node
	    {   MPI_Send(buff, limit, MPI_CHAR,
		    dest, tag, MPI_COMM_WORLD);
	    } // end for
	    printf("\n[Swarm brent] Processing with %d cores\n", cores);
	}else				    		    // a compute node
	{   MPI_Recv(buff, limit, MPI_CHAR,
		source, tag, MPI_COMM_WORLD, &status);	    // receive buff
	}  //end if/else

	/* Algorithm ************************************************************/
	mpz_set_str(n, buff, 10);		    	    // move buff to 'n'
	mpz_add(seed, TWO, CORES);		    	    // simple start
	while(prbrent(factor, n, seed) == 0)		    // fail: bad number
		mpz_add(seed, seed, CORES);	  	    // new number

	/* Output ***************************************************************/
	printf("rank %d factor = ", rank);	    	    // each core reports
	mpz_out_str(stdout, BASE, factor);    printf("\n");

	/* Clean up *************************************************************/
	MPI_Finalize();				    	    // shut down MPI
	return 0;
} // end main
