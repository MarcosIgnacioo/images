#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#define BMP_HEADER_SIZE 54
#define BMP_COLOR_TABLE_SIZE 1024
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

FILE *save_in_header(BMP_Header *header, const char *image_path) {
  FILE *stream_in;
  stream_in = fopen(image_path, "r");
  if (!stream_in)
    return NULL;
  fread(header, sizeof(BMP_Header), 1, stream_in);
  if (ferror(stream_in)) {
    fclose(stream_in);
    perror("error saving header");
    return NULL;
  }
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

int get_image_size(BMP_Header header) {
  return header.width * header.height * (header.bits_depth / 8);
}

typedef struct {
  BMP_Header header;
  unsigned char *color_table;
  unsigned char *image_data;
  unsigned char *foo;
  FILE *file_stream;
  int height;
  int width;
  int size;
} BMP_Image;

void free_img(BMP_Image *img) {
  if (img->color_table) {
    free(img->color_table);
  }
  if (img->image_data) {
    free(img->image_data);
  }
  fclose(img->file_stream);
}

BMP_Image new_bmp_image(const char *file_path) {

  BMP_Header header = {0};
  FILE *stream_in = save_in_header(&header, file_path);
  unsigned char *image_data = NULL;
  unsigned char *color_table = NULL;

  if (!stream_in)
    return (BMP_Image){0};

  int size = get_image_size(header);

  image_data = malloc(size);

  if (!image_data)
    return (BMP_Image){0};

  if (header.bits_depth <= 8) {
    color_table = malloc(BMP_COLOR_TABLE_SIZE);

    if (!color_table) {
      free(image_data);
      image_data = NULL;
      color_table = NULL;
      return (BMP_Image){0};
    }
    fread(color_table, sizeof(unsigned char), BMP_COLOR_TABLE_SIZE, stream_in);
  }

  if (ferror(stream_in)) {
    free(color_table);
    free(image_data);
    image_data = NULL;
    color_table = NULL;
    fclose(stream_in);
    perror("error saving color table");
    return (BMP_Image){0};
  }

  fread(image_data, sizeof(unsigned char), size, stream_in);

  if (ferror(stream_in)) {
    free(color_table);
    free(image_data);
    image_data = NULL;
    color_table = NULL;
    fclose(stream_in);
    perror("error saving image data");
    return (BMP_Image){0};
  }

  return (BMP_Image){.header = header,
                     .color_table = color_table,
                     .image_data = image_data,
                     .foo = color_table,
                     .size = size,
                     .width = header.width,
                     .height = header.height,
                     .file_stream = stream_in};
}

char *get_rand_name(const char *name) {
  int r = rand() % 10000;
  char *rand_name = malloc(256);
  if (!rand_name)
    return NULL;
  sprintf(rand_name, "%s%d.bmp", name, r);
  return rand_name;
}

void write_image(BMP_Image *img, const char *name) {

  FILE *our_bmp = fopen(name, "wb");
  int is_error = 0;
  if (!our_bmp)
    return;

  is_error =
      fwrite(&img->header, sizeof(unsigned char), BMP_HEADER_SIZE, our_bmp);

  if (is_error < 1 || ferror(our_bmp)) {
    fclose(our_bmp);
    perror("error setting bmp header");
    return;
  }

  if (img->header.bits_depth <= 8) {
    is_error = fwrite(img->color_table, sizeof(unsigned char),
                      BMP_COLOR_TABLE_SIZE, our_bmp);
  }

  if (is_error < 1 || ferror(our_bmp)) {
    fclose(our_bmp);
    perror("error setting color table");
    return;
  }

  is_error = fwrite(img->image_data, sizeof(unsigned char), img->size, our_bmp);

  if (is_error < 1 || ferror(our_bmp)) {
    fclose(our_bmp);
    perror("error setting image data");
    return;
  }

  fclose(our_bmp);
}

int main() {
  BMP_Header h = {0};
  BMP_Image img = new_bmp_image("./linux.bmp");
  char *linus = get_rand_name("./poop/linuxfinalboss");
  BMP_Image img2 = new_bmp_image("./lena512.bmp");
  char *lena = get_rand_name("./poop/lenafinalboss");
  write_image(&img, linus);
  //
  write_image(&img2, lena);
  free_img(&img);
  free_img(&img2);
  free(linus);
  free(lena);
  return 0;
}
