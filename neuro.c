#include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

typedef struct neuron neuron;
struct neuron {
	size_t size;
	double *weights;
	double *inputs;
	double bias;
	double error;
};

/* La fonctino malloc retourne un pointeur sur le premier byte allouer. Sizeof donne la taille de l'element dans le tab, c'est à dire la m taille qu'un double * size.
   La fonction rand donne un nombre aleatoire, pour avoir un nombre en a et b cela doit etre sous la forme rand()/(double)RAND_MAX * (b-a) + a, or nous n'avons pas de a et b, la valeur est entre 0 et 1.
*/
struct neuron neuron_init(size_t size, double weights, double inputs ) {
	  neuron n;
	  n.weights = malloc(size * sizeof(double)); 
	  n.inputs = malloc(size * sizeof(double));
	  for (size_t i = 0; i < size; i++) {
	    n.weights[i] = (double)rand()/(double)RAND_MAX;
	  }  
	  n.size = size;
          n.bias = (double)rand()/(double)RAND_MAX;
          return n;
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

int main (int argc, char *argv[]){  

}	
