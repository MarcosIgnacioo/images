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

FILE *save_in_header(BMP_Header *header, char *image_path) {
  FILE *stream_in;
  stream_in = fopen(image_path, "r");
  fread(header, sizeof(BMP_Header), 1, stream_in);
  return stream_in;
}

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

void write_image(const char *name, BMP_Header *header,
                 const unsigned char *image_data, const int size) {
  FILE *our_bmp = fopen(name, "wb");
  fwrite(header, sizeof(unsigned char), 54, our_bmp);
  fwrite(image_data, sizeof(unsigned char), size, our_bmp);
  fclose(our_bmp);
}

int get_image_size(BMP_Header header) {
  return header.width * header.height * (header.bits_depth / 8);
}

int main() {
  BMP_Header header = {0};
  srand(time(NULL));
  int r = rand() % 10000;
  char name[256];
  FILE *stream_in = save_in_header(&header, "./linux.bmp");
  int size = get_image_size(header);
  unsigned char image_data[size];

  fread(image_data, sizeof(unsigned char), size, stream_in);
  sprintf(name, "linux%d.bmp", r);
  write_image(name, &header, image_data, size);
  printf("%d \n", header.bits_depth);
  fclose(stream_in);
}

// rewind(stream_in)
// k
// fread(&h, sizeof(BMP_Header), 1, stream_in);
// printf("%d \n", h.bits_depth);
