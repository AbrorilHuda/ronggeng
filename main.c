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
  scanf("%1s", inputan2);
  // printf("result data %c %c\n", inputan[1], inputan[1]);
  if (inputan2[0] == 'y') {
    int inputModif;
    printf("masukkan umur mu langsung: ");
    scanf("%d", &inputModif);
    calculate(inputModif, myyear);
    char pertanyaan[2];
    printf("apakah umur anda mau di kuadratkan: (y/n)");
    scanf("%s", pertanyaan);
    if (pertanyaan[0] == 'y') {
      int inputankuadrat;
      printf("masukkan angka berapa untuk nilai Y nya: ");
      scanf("%d", &inputankuadrat);
      int resultkuadrat = kuadrat(inputModif, inputankuadrat);
      printf("ini hasil kuadratnya %d\n", resultkuadrat);
    } else {
      printf("byee\n");
    }
  } else {
    printf("byeeee\n");
  }
  return 0;
}
