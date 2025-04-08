#include "cekumur.c"
#include <stdio.h>

int main() {

  int numberku = 12;
  char myName = 'A';
  char name[] = "Abrordc";
  printf("hello world \n");
  printf("ini adalah numberku: %d\n", numberku);
  printf("ini adalah char: %c %s\n", myName, name);

  // cek bytes
  int number;
  float floatdata;
  double doubledata;
  char string;

  printf("%lu\n", sizeof(number));
  printf("%lu\n", sizeof(floatdata));
  printf("%lu\n", sizeof(doubledata));
  printf("%lu\n", sizeof(string));

  int myyear;
  printf("masukkan tahun lahir anda: ");
  scanf("%d", &myyear);
  calculateAge(myyear);
  char inputan2[2];
  printf("hapus umur atau modif: (y/n):");
  scanf("%s", inputan2);
  char inputan[2] = "y";
  if (inputan2[1] == inputan[1]) {
    int inputModif;
    printf("masukkan umur mu langsung: ");
    scanf("%d", &inputModif);
    calculate(inputModif, myyear);
  }
  return 0;
}
