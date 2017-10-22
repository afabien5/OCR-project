# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <math.h>
# include "matrix.h"

typedef struct neuron neuron;
struct neuron 
{
	size_t size;
	double *weights;
	double entree;
	double sortie;
	double biais;
	double error;
};
typedef struct Network Network;
struct Network
{
	neuron *v1;
	neuron *v2;
	neuron *v3;
	size_t *l;
	size_t inter;
	double *z2;
	double *a2;
	double *z3;
	double *a3;

};

neuron neuron_init(size_t size);

Network network_init(size_t v1, size_t v2, size_t v3);

void print_network(Network r);

double signoid(double x);

double derivate(double x);

void output(neuron *v1, neuron *v2, size_t l1, size_t l2);

double parcours(Network r,double imput1, double imput2);

double cost(Network r);

Network update_network(Network r);

