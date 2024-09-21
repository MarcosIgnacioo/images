#define _POSIX_C_SOURCE                                                        \
  199309L // Definir POSIX_C_SOURCE para incluir clock_gettime
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct {
  short int signature __attribute__((packed));
  int size __attribute__((packed));
  short int reserved_1 __attribute__((packed));
  short int reserved_2 __attribute__((packed));
  int offset __attribute__((packed));
  int bitmap_info_header_size __attribute__((packed));
  int width __attribute__((packed));
  int height __attribute__((packed));
  short int planes __attribute__((packed));
  short int bits_depth __attribute__((packed));
  int compression_type __attribute__((packed));
  int size_of_image_data __attribute__((packed));
  int horizontal_resolution __attribute__((packed));
  int vertical_resolution __attribute__((packed));
  int number_of_colors __attribute__((packed));
  int number_of_important_colors __attribute__((packed));
} BMP_Header;

// From the above header, the information really important to us are width,
// height and bitDepth.

void print_string_liner(unsigned char *str, int strlen) {
  for (int i = 0; i < strlen; i++) {
    char *space = (i <= 9) ? "  " : " ";
    printf("b%d:%s%u ", i, space, (unsigned int)str[i]);
    if (str[i] < 9) {
      printf(" ");
    }
    if (i > 0 && i % 4 == 0) {
      printf("\n");
    }
  }
}

int main() {
  unsigned char image_header[54];
  unsigned char color_table[1024];
  // BMP_Header h = {0};
  FILE *stream_in;

  stream_in =
      // no tengo que usar abs path si pongo la imgagen en la carpeta out aka
      // donde se esta ejecutando el biarinirio por lo que puedo usar ./
      fopen("./linux.bmp", "r");
  for (int i = 0; i < 54; i++) {
    image_header[i] = getc(stream_in);
  }
  print_string_liner(image_header, 54);
  // lo que hacemos aqui obtener la direccion en memoria del 18 byte del header,
  // despues casteamos esto a un int pointer y cuando lo dereferenciemos el
  // compilador lo va a leer como un numero los bytes estan asi
  // bytes (index en el header)
  // 1 (18)    2 (19)     3 (20)    4 (21)
  // 0000 0000 01000 0000 0000 0000 0000 0000 00000
  // 0123 4567 89
  // 2^9 = 512
  int width = *(int *)&image_header[18];
  printf("%d \n", width);
  int height = *(int *)&image_header[22];
  printf("%d \n", height);
  int bit_depth = *(int *)&image_header[28];

  if (bit_depth <= 8) {
    fread(color_table, sizeof(unsigned char), 1024, stream_in);
  }

  unsigned char image_data[(width * height)];
  fread(image_data, sizeof(unsigned char), height * width, stream_in);
  srand(time(NULL)); // Initialization, should only be called once.
  int r = rand() % 10000;
  char name[256];
  sprintf(name, "linux%d.bmp", r);
  FILE *our_bmp = fopen(name, "wb");
  fwrite(image_header, sizeof(unsigned char), 54, our_bmp);

  if (bit_depth <= 8) {
    fwrite(color_table, sizeof(unsigned char), 1024, our_bmp);
  }

  fwrite(image_data, sizeof(unsigned char), width * height, our_bmp);
  fclose(stream_in);
  fclose(our_bmp);
  // printf("\n");
  // printf("%d \n", width);
  // printf("%d \n", height);
  // printf("%d \n", bit_depth);
  return 0;
}
// rewind(stream_in)
// k
// fread(&h, sizeof(BMP_Header), 1, stream_in);
// printf("%d \n", h.bits_depth);
