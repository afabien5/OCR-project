#ifndef _SEGMENTATION_H_
#define _SEGMENTATION_H_

struct Matrix {
  int *M;
  int width;
  int height;
};

typedef void (*Callback)(struct Matrix M, int x1, int x2, int y1, int y2);
// The matrix M may be freed after the call returns, so if you want to keep a
// copy of it then you need to allocate your own matrix to hold the copy.

struct Callbacks {	//contient des fonctions de type Callback
  Callback start_line; 
  Callback stop_line;
  Callback start_word;	//les noms sont obvious
  Callback stop_word;
  Callback start_letter;
  Callback stop_letter;
};

int height(struct Matrix M);
int width(struct Matrix M);
int in(struct Matrix M, int x, int y);
int get(struct Matrix M, int x, int y);
int getcolor(struct Matrix M, int x, int y);
void set(struct Matrix M, int x, int y, int value);
void highlight(struct Matrix M, int x, int y, int n);
int is_blank(int value);

void ReadText(struct Matrix M, int spacing, struct Callbacks callbacks);
void PrintMatrix(struct Matrix M);

#endif
