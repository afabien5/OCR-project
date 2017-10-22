# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>
# include <time.h>
# include "matrix.h"
typedef struct neuron neuron;
struct neuron {
	size_t size;    //number of neuron imputs.
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
	  struct neuron n;
	  n.entree = 0;
	  n.sortie = 0;
	  double b[size * sizeof(double)];
	  n.weights =b;

	   

	  for (size_t i = 0; i < size; i++) {
	   *(n.weights+i) = (double)rand()/(double)RAND_MAX; 
	  }
	  
	  
	  n.size = size;
	  

          n.bias = (double)rand()/(double)RAND_MAX;
		
	  printf("ok\n");
	  return n;
}
Network network_init(size_t v1, size_t v2, size_t v3)
{
	Network r;
	
	size_t lenB[3 * sizeof(size_t)];
	r.l = lenB;
	
	*(r.l) = v1;
        *(r.l+1) = v2;
	*(r.l+2) = v3;


	neuron couche1[v1* sizeof(neuron)];
	r.v1 = couche1;
	neuron couche2[v2* sizeof(neuron)];
       	r.v2 = couche2;
	neuron couche3[v3* sizeof(neuron)];
	r.v3 = couche3;
	
	srand(time(NULL));
	for(size_t inter = 0;inter<v1 ;inter++)
	{
		*(r.v1 +inter) = neuron_init(1);

	}
	for(size_t inter = 0;inter<v2 ;inter++)
        {
		*(r.v2 +inter) = neuron_init(2); 
       	}
	for(size_t inter = 0;inter<v3 ;inter++)
       	{
		*(r.v3 +inter) = neuron_init(2);   
	}
	
	return r;
	
}

void print_network(Network r)
{
       	printf("\n");
	printf("%lf/%lf  %lf/%lf\n",r.v1[0].entree,r.v1[0].sortie,r.v2[0].entree,r.v2[0].sortie);
	printf("                                    %lf/%lf\n ",r.v3[0].entree, r.v3[0].sortie);
	printf("%lf/%lf  %lf/%lf\n",r.v1[1].entree,r.v1[1].sortie,r.v2[1].entree,r.v2[1].sortie);
	printf("\n--------------------------------------------------------\n");
}

/*void print_network(Network r)
{
	printf("Reseau: \n");
	printf("Entrer/Sortie v1/neuron1: %lf/%lf\nEntrer/Sortie v2/neuron1: %lf/%lf\n",r.v1[0].entree,r.v1[0].sortie,r.v2[0].entree,r.v2[0].sortie);
	printf("Entrer/Sortie v3:         %lf/%lf\n",r.v3[0].entree, r.v3[0].sortie); 
     	printf("Entrer/Sortie v1/neuron2: %lf/%lf\nEntrer/Sortie v2/neuron2: %lf/%lf\n",r.v1[1].entree,r.v1[1].sortie,r.v2[1].entree,r.v2[1].sortie); 
	printf("\n--------------------------------------------------------\n");
}*/

/*void print_neuron(neuron n) {
	for(size_t i = 0; i<l1; ++i){
		
		for(size_t j = 0 ; j<l2; ++j){
		       	
		}
 	}
}*/


double signoid(double x) {
	  return 1 / (1 + exp(-x));
}

void signoid_n(neuron *n)
{
	        (*n).sortie = (*n).entree + (*n).bias ;
		
}

double derivate(double x) {
	  return x * (1 - x);
}



/*double output(double input[], double weights[], double bias, size_t size) {
	  double retour = 0;
	    for(size_t i = 0; i < size; i++) {
		        retour += input[i] * weights[i];
			  }
	      return signoid(retour + bias);
}*/


		/* Parcours du reseau */ 	

/*double parcours(Network r,double imput1, double imput2) {
	r.inter = 0;
	r.v1[0].sortie = imput1;
	print_network(r);
	r.v1[1].sortie = imput2;
	print_network(r);

	output(r.v1,r.v2,r.l[0],r.l[1]);
	print_network(r);

       	output(r.v2,r.v3,r.l[1],r.l[2]);
	print_network(r);


	return(r.v3[0].sortie);

}*/

void output(neuron *v1, neuron *v2, size_t l1, size_t l2){
	
	for(size_t i = 0; i<l1; ++i)
	{       
		for(size_t j = 0 ; j<l2; ++j)
		{
			(v2+j)->entree += (v1+i)->sortie * (v2+j)->weights[i];
			printf("%lf ",(v2+j)->entree);
			
		}

	}
	for(size_t j = 0 ; j<l2; ++j ) 
	{
	
		(v2+j)->entree += (v2+j)->bias;
		//printf("%lf",(v2+j)->entree);
		(v2+j)->sortie = signoid((v2+j)-> entree );
		//printf("%lf",(v2+j)->sortie);


	}
}

/*void output1(neuron *v1, neuron *v2, size_t l1, size_t l2, size_t w) {
	size_t lencouche = 2
	for( size_t i = 0, i < l1, ++i) {
		for(size_t j = 0, j < l2, j++)
		{
			v1[i]
		}
	}
}*/
double parcours(Network r,double imput1, double imput2) {
	//r.inter = 0;
	
	r.v1[0].sortie = imput1;

	r.v1[1].sortie = imput2;
	print_network(r);
	output((r.v1),(r.v2),2,2);
	
	print_network(r);
	output(r.v2,r.v3,2,1);
	print_network(r);
	return(r.v3[0].sortie);

}





