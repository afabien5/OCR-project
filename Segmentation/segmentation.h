#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

struct Matrix {
  int *M;
  int width;
  int height;
};

void ReadText(struct Matrix M, int spacing);
