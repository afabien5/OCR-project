#include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include "matrix.h"
typedef struct neuron neuron;
struct neuron {
	size_t size;
	double *weights;
	double entree;
	double sortie;
	double bias;
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
};

/* La fonctino malloc retourne un pointeur sur le premier byte allouer. Sizeof donne la taille de l'element dans le tab, c'est à dire la m taille qu'un double * size.
   La fonction rand donne un nombre aleatoire, pour avoir un nombre en a et b cela doit etre sous la forme rand()/(double)RAND_MAX * (b-a) + a, or nous n'avons pas de a et b, la valeur est entre 0 et 1.
*/
neuron neuron_init(size_t size) {
	  neuron n;
	  n.weights = malloc(size * sizeof(double)); 
	 
	  for (size_t i = 0; i < size; i++) {
	   *(n.weights+i) = (double)rand()/(double)RAND_MAX;
	  }  
	  n.size = size;
          n.bias = (double)rand()/(double)RAND_MAX;
          return n;
}
Network network_init(size_t v1, size_t v2, size_t v3)
{
	Network r;
	r.l = malloc(3 *sizeof(size_t));
	*(r.l) = v1;
        *(r.l+1) = v2;
	*(r.l+2) = v3;
	r.v1 = malloc(v1 * sizeof(neuron));
       	r.v2 = malloc(v2 * sizeof(neuron));
	r.v3 = malloc(v3 * sizeof(neuron));
	
		
	for(size_t inter = 0;inter<v1 ;inter++)
	{
		*(r.v1 +inter) = neuron_init(0);

	}
	for(size_t inter = 0;inter<v2 ;inter++)
        {
		*(r.v1 +inter) = neuron_init(*(r.l));
       	}
	for(size_t inter = 0;inter<v1 ;inter++)
       	{
		*(r.v1 +inter) = neuron_init(0);
	}

	return r;


}

double signoid(double x) {
	  return 1 / (1 + exp(-x));
}


double derivate(double x) {
	  return x * (1 - x);
}




double output(double input[], double weights[], double bias, size_t size) {
	  double retour = 0;
	    for(size_t i = 0; i < size; i++) {
		        retour += input[i] * weights[i];
			  }
	      return signoid(retour + bias);
}

int main ()
{ 
       	printf("hello");
/*	
	Network r = network_init(2,2,1);
	for(size_t i = 0 ; i<2 ; i++)
	{
		for(size_t j = 0 ; j<2; ++j)
		{
		printf("%lf ",*(r.v2[i].weights+j));

	
		}
		printf("\n");
	}*/
	return 0;

}	
