#include <math.h>
#include <stdio.h>

void calculateAge(int input) {
  int result = 2025 - input;
  printf("umur mu sekarang : %d\n", result);
}

void calculate(int data, int dataAwal) {
  // clang-format off
  int* sementara = &dataAwal;
  *sementara = data;

  printf("umur sudah di modif %d\n", *sementara);
}

int kuadrat(int x, int y){
    int proses = pow(x, y);
    return proses;
}
