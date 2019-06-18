#define TIMES 1000

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <thread>
#include <fstream>
#include <stdint.h>


using namespace std;


const float sec_const = 1000000.0;

typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;  // WORD = unsigned 16 bit value
typedef unsigned long LONG;   

#pragma pack(push, 1)

typedef struct tagBITMAPFILEHEADER
{
  WORD bfType;  //specifies the file type
  DWORD bfSize;  //specifies the size in bytes of the bitmap file
  WORD bfReserved1;  //reserved; must be 0
  WORD bfReserved2;  //reserved; must be 0
  DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER __attribute__((__packed__));

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;  //specifies the number of bytes required by the struct
  DWORD biWidth;  //specifies width in pixels
  DWORD biHeight;  //species height in pixels
  WORD biPlanes; //specifies the number of color planes, must be 1
  WORD biBitCount; //specifies the number of bit per pixel
  DWORD biCompression;//spcifies the type of compression
  DWORD biSizeImage;  //size of image in bytes
  DWORD biXPelsPerMeter;  //number of pixels per meter in x axis
  DWORD biYPelsPerMeter;  //number of pixels per meter in y axis
  DWORD biClrUsed;  //number of colors used by th ebitmap
  DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER __attribute__((__packed__));

#pragma pack(pop)

unsigned char *LoadBitmapFile(const char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
  FILE *filePtr; //our file pointer
  BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
  unsigned char *bitmapImage;  //store image data
  int imageIdx=0;  //image index counter
  unsigned char tempRGB;  //our swap variable

  //open filename in read binary mode
  filePtr = fopen(filename, "rb");
  if (filePtr == NULL)
    return NULL;

  //read the bitmap file header
  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

  //verify that this is a bmp file by check bitmap id
  if (bitmapFileHeader.bfType !=0x4D42)
  {
    fclose(filePtr);
    return NULL;
  }

  //read the bitmap info header
  fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

  //move file point to the begging of bitmap data
  fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

  //allocate enough memory for the bitmap image data
  bitmapImage = (unsigned char *) malloc(bitmapInfoHeader->biSizeImage);

  //verify memory allocation
  if (!bitmapImage)
  {
    free(bitmapImage);
    fclose(filePtr);
    return NULL;
  }

  //read in the bitmap image data
  fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

  //make sure bitmap image data was read
  if (bitmapImage == NULL)
  {
    fclose(filePtr);
    return NULL;
  }

  //swap the r and b values to get RGB (bitmap is BGR)
  for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3) // fixed semicolon
  {
    tempRGB = bitmapImage[imageIdx];
    bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
    bitmapImage[imageIdx + 2] = tempRGB;
  }

  //close file and return bitmap image data
  fclose(filePtr);
  return bitmapImage;
}

unsigned char find_min0(unsigned char* one_channel, uint32_t channel_size){
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  unsigned char result;
  
  start_t = clock();

  // for (int j; j < 1000000000; j++){
	result = 255;  // maximum
	  for (uint32_t i = 0; i < channel_size; i++)
	    if (one_channel[i] < result)
	      result = one_channel[i];
  // }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("1 thread.\t %.2f \t\n", clock_delta_sec);

  return result;
}

void find_min1_inner(unsigned char & result, unsigned char* one_channel, uint32_t start, uint32_t end) {
  // unsigned char & res = const_cast<unsigned char &>(result);
  result = 255;
  for (uint32_t i = start; i < end; i++)
    if (one_channel[i] < result)
      result = one_channel[i];
}

unsigned char find_min1(unsigned char* one_channel, uint32_t channel_size){
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;
  uint16_t N_THREADS = 8;
  unsigned char result;
  
  start_t = clock();

  // ToDo
  uint32_t chunk_size = int(channel_size / N_THREADS);

  // for (int j; j < 100000; j++){
    unsigned char *results = (unsigned char *) malloc(N_THREADS * sizeof(unsigned char));
	thread threads[N_THREADS];

	for (uint16_t i = 0; i < N_THREADS - 1; i++)
	  threads[i] = thread(
	    find_min1_inner, 
	    ref(results[i]), 
	    ref(one_channel),
	    i * chunk_size,
	    (i + 1) * chunk_size
	  );
	threads[N_THREADS - 1] = thread(
	  find_min1_inner,
	  ref(results[N_THREADS - 1]),
	  ref(one_channel),
	  (N_THREADS - 1) * chunk_size,
	  channel_size
	);

    result = 255;
	for (uint16_t i = 0; i < N_THREADS; i++){
	  threads[i].join();
	  if (results[i] < result)
	    result = results[i];
	}
  // }

  end_t = clock();
  clock_delta = end_t - start_t;
  clock_delta_sec = (double) (clock_delta / sec_const);
  printf("%i threads.\t %.2f \t\n", N_THREADS, clock_delta_sec);

  return result;
}


int main(){
  const char *filepath = "../task2/MARBLES.BMP";
  BITMAPINFOHEADER *bitmapInfoHeader;
  unsigned char *bitmapData;
  unsigned char *greenData;
  uint32_t image_size;
  uint32_t image_width;
  uint32_t image_height;
  uint32_t channel_size;

  bitmapInfoHeader = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
  bitmapData = LoadBitmapFile(filepath, bitmapInfoHeader);  // /content/drive/My Drive/UCU/PerfEng/dev/performance-engineering-course
  //Here we have 3 channels of image
  // printf("%x, %x, %x \n", bitmapData[0], bitmapData[1], bitmapData[2]);  // first RGB data

  image_size = bitmapInfoHeader->biSizeImage;
  image_width = bitmapInfoHeader->biWidth;
  image_height = bitmapInfoHeader->biHeight;
  channel_size = image_size / 3;
  //Here we have 3 channels of image

  greenData = new unsigned char[channel_size * TIMES];

  for(int j = 0; j < TIMES; j++)  // for multiply a channel
    for(unsigned long i = 0; i < channel_size; i++)
      greenData[j * channel_size + i] = bitmapData[3 * i + 1];  // let's calc for G - Green color

  printf("Function \t Time(Sec) \n");
  unsigned char r1 = find_min0(greenData, channel_size * TIMES);
  unsigned char r2 = find_min1(greenData, channel_size * TIMES);

  printf("MIN is: %c & %c", r1, r2);

  //FREE MEM
  free(bitmapData);
  free(bitmapInfoHeader);
  return 0;
}
