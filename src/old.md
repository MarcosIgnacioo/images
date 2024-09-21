```c
unsigned char image_header[54];
unsigned char color_table[1024];
FILE *stream_in;

stream_in =
    fopen("./linux.bmp", "r");
for (int i = 0; i < 54; i++) {
  image_header[i] = getc(stream_in);
}
print_string_liner(image_header, 54);

header,
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
srand(time(NULL)); 
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
return 0;
```
