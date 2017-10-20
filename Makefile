Programme : main.o neuro.o
	gcc  main.o neuro.o -o Programme

main.o : main.c neuro.c
	gcc -c main.c -o main.o

fonctions.o : neuro.c
	gcc -c fonctions.c -o fonctions.o  ions.c -o fonctions.o
