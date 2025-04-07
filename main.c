#include "cekumur.c"
#include <stdio.h>

int
main ()
{
  int numberku = 12;
  char myName = 'A';
  char name[] = "Abrordc";
  printf ("hello world \n");
  printf ("ini adalah numberku: %d\n", numberku);
  printf ("ini adalah char: %c %s\n", myName, name);

  // cek byte
  int number;
  float floatdata;
  double doubledata;
  char string;

  printf ("%lu\n", sizeof (number));
  printf ("%lu\n", sizeof (floatdata));
  printf ("%lu\n", sizeof (doubledata));
  printf ("%lu\n", sizeof (string));

  int myyear;
  printf ("masukkan tahun lahir anda: ");
  scanf ("%d", &myyear);
  calculateAge (myyear);
  return 0;
}
