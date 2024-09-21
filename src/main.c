#include <stdio.h>
#include <stdlib.h>
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

void print_string_liner(char *str, int strlen) {
  for (int i = 0; i < strlen; i++) {
    char *space = (i <= 9) ? "  " : " ";
    printf("b%d:%s%d ", i, space, str[i]);
    if (str[i] < 9) {
      printf(" ");
    }
    if (i > 0 && i % 4 == 0) {
      printf("\n");
    }
  }
}

int main() {
  char image_header[54];
  char color_table[1024];
  BMP_Header h = {0};
  FILE *stream_in;
  stream_in =
      fopen("/home/happy/farhampton/marcig/images/src/lena512.bmp", "r");
  fread(&h, sizeof(BMP_Header), 1, stream_in);
  printf("%d \n", h.bits_depth);
  return 0;
}
