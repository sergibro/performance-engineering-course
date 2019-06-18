#define TIMES 1000

#include <mpi.h>
#include <stdio.h>
#include <time.h>
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

unsigned char find_min1_inner(unsigned char* one_channel, uint32_t start, uint32_t end) {
  unsigned char result = 255;
  for (uint32_t i = start; i < end; i++)
    if (one_channel[i] < result)
      result = one_channel[i];
  return result;
}

unsigned char find_min1(unsigned char* one_channel, uint32_t channel_size, int myid, int num_procs){
  unsigned char curr_min = 255;
  unsigned char result = 255;
  MPI_Status status;
  uint32_t chunk_size = int(channel_size / num_procs);
  if (myid < num_procs)
    curr_min = find_min1_inner(one_channel, myid * chunk_size, (myid + 1) * chunk_size);
  else
    curr_min = find_min1_inner(one_channel, myid * chunk_size, channel_size);
  result = curr_min;
  if (myid == 0) {
    for (int i = 1; i < num_procs; i++) {
      MPI_Recv(&curr_min, 1, MPI_UINT64_T, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      if (curr_min < result)
        result = curr_min;
    }
  }
  else
    MPI_Send(&curr_min, 1, MPI_UINT64_T, 0, 1, MPI_COMM_WORLD);
  return result;
}


int main(int argc, char *argv[]){
  int n, myid, numprocs, i;
  BITMAPINFOHEADER *bitmapInfoHeader;
  unsigned char *bitmapData;
  unsigned char *greenData;
  uint32_t channel_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  clock_t start_t;
  clock_t end_t;
  clock_t clock_delta;
  double clock_delta_sec;

  if (myid == 0) {
  	const char *filepath = "../task2/MARBLES.BMP";
    uint32_t image_size;
    uint32_t image_width;
    uint32_t image_height;

    bitmapInfoHeader = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
    bitmapData = LoadBitmapFile(filepath, bitmapInfoHeader);  // /content/drive/My Drive/UCU/PerfEng/dev/performance-engineering-course
    //Here we have 3 channels of image
    // printf("%x, %x, %x \n", bitmapData[0], bitmapData[1], bitmapData[2]);  // first RGB data

    image_size = bitmapInfoHeader->biSizeImage;
    image_width = bitmapInfoHeader->biWidth;
    image_height = bitmapInfoHeader->biHeight;
    channel_size = image_size / 3;
    //Here we have 3 channels of image
  }

  MPI_Bcast(&channel_size, 1, MPI_UINT32_T, 0, MPI_COMM_WORLD);
  greenData = (unsigned char *) malloc(channel_size * TIMES * sizeof(unsigned char));  // new unsigned char[channel_size * TIMES];

  if (myid == 0) {
    for(int j = 0; j < TIMES; j++)  // for multiply a channel
      for(unsigned long i = 0; i < channel_size; i++)
        greenData[j * channel_size + i] = bitmapData[3 * i + 1];  // let's calc for G - Green color
    start_t = clock();
  }

  MPI_Bcast(greenData, channel_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

  unsigned char r = find_min1(greenData, channel_size * TIMES, myid, numprocs);
  
  if (myid == 0){
  	end_t = clock();

    clock_delta = end_t - start_t;
    clock_delta_sec = (double) (clock_delta / sec_const);
    printf("MPI time \t %.2f \t\n", clock_delta_sec);
	printf("MIN is: %c", r);

    // FREE MEM
    free(bitmapData);
    free(bitmapInfoHeader);
  }
  
  return 0;
}
