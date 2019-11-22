#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h> 


#define kInputFileName "nine.three.wav"
#define kOutputFileName "9.3.wav"
#define kBufferSize 4096
#define saturate(x) __min(__max(-1.0,x),1.0)
#define kLevel 0.9
#define keepBits 16

/*Compile this code using:

gcc ProblemSet9.3.c -o 9.3 -lsndfile && ./9.3

*/

float BassBoosta(float sample)
{
static float cap;
static float gain2 = 0.8;
static float ratio = 0.7;
static float selectivity = 100;
float gain1 = 1.0/(selectivity + 1.0);

cap= (sample + cap*selectivity )*gain1;
sample = (sample + cap*ratio)*gain2;
//had to change this line to make function work

return sample;
}

//actual audio data + file meta data
typedef struct SoundFile {
  SNDFILE *file;
  SF_INFO info;
} SoundFile;

//declarations listed at bottom of code
int openInputSndFile(SoundFile*);
int createOutputSndFile(SoundFile *inFile, SoundFile *outFile);
void process(float *inBuffer, float *outBuffer, sf_count_t bufferSize);
//short keep_bits_from_16(short input, int keepBits ) {
 // return (input & (-1 << (16-keepBits)));
//}
//---------------------------------------------------------------------------------------------
int main(void){

float gain2 = 0.8;
float ratio = 0.7;
float selectivity = 100;
float gain1 = 1.0/(selectivity + 1.0);
  SoundFile inFile, outFile;
  //int keepBits = 10;
  
  //Open input file and create output file
  int error = openInputSndFile(&inFile);

  if(error) return 1;

  //get buffer size
  sf_count_t bufferSize = inFile.info.frames;

  //allocate memory in buffers
  float *inBuffer = (float *) malloc(bufferSize*sizeof(float));
  float *outBuffer = (float *) calloc(bufferSize,sizeof(float));

  //copy input into buffer
  sf_read_float(inFile.file, inBuffer, bufferSize);

  //actual effect processing part //process details below
  //for(sf_count_t n = 0; n < bufferSize; n++){
  //inBuffer[n] = (inBuffer[n] & (-1 << (16-keepBits)));
  process(inBuffer, outBuffer, bufferSize);
  
  //create output file and write the result
  error = createOutputSndFile(&inFile, &outFile);
  if(error) return 1;
  sf_write_float(outFile.file, outBuffer, bufferSize);
  
  //close files
  sf_close(inFile.file);
  sf_close(outFile.file);
  free(inBuffer);
  free(outBuffer);
  
  return 0;
}

void process(float *inBuffer, float *outBuffer, sf_count_t bufferSize){
  sf_count_t m;
 float *idk;
  for(sf_count_t n = 0; n < bufferSize; n++){
    outBuffer[n] = BassBoosta(inBuffer[n]);
    //outBuffer[n] = keep_bits_from_16(inBuffer[n], 16);//maybe this will work
    //idk[n] = keep_bits_from_16(inBuffer[n], 16);//maybe this will work
    //printf("%f");
    //outBuffer[n] *= kLevel; //Adjust volume
  }
}

int openInputSndFile(SoundFile *sndFile){
  //Initialize SF_INFO with 0s (Required for reading)
  memset(&sndFile->info, 0, sizeof(SF_INFO));

  //Open the original sound file as read mode
  sndFile->file = sf_open(kInputFileName, SFM_READ, &sndFile->info);
  if(!sndFile->file){//Check if the file was succefully opened
    printf("Error : could not open file : %s\n", kInputFileName);
		puts(sf_strerror(NULL));
		return 1;
  }

  //Check if the file format is in good shape
  if(!sf_format_check(&sndFile->info)){	
    sf_close(sndFile->file);
		printf("Invalid encoding\n");
		return 1;
	}

  //Check if the file is mono
  if(sndFile->info.channels > 1){
    printf("Input file is not mono\n");
    return 1;
  }

  //print out information about opened sound file
  printf("Sample rate for this file is %d\n", sndFile->info.samplerate);
	printf("A number of channels in this file is %d\n", sndFile->info.channels);
  printf("A number of frames in this file is %lld\n", sndFile->info.frames);
  printf("time is %f\n", (double)sndFile->info.frames / sndFile->info.samplerate);

  return 0;
}

int createOutputSndFile(SoundFile *inFile, SoundFile *outFile){
  //Open another sound file in write mode
  outFile->file = sf_open(kOutputFileName, SFM_WRITE, &inFile->info);
  if(!outFile->file){//Check if the file was succefully opened
    printf("Error : could not open file : %s\n", kOutputFileName);
		puts(sf_strerror(NULL));
		return 1;
	}
  return 0;
}