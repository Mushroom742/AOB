/*
  This code performs color invertion to compute a mask of a video stream meant as input to a neural network
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

//
#include "rdtsc.h"

//
#include "common.h"

//
#include <omp.h>
#define NUM_THREADS 4

//
void process_baseline(u8 *frame)
{
  for (unsigned y = 0 ; y < H ; ++y)
    for (unsigned x = 0 ; x < W * 3 ; x += 3)
      {
	//Invert each colour component of every pixel
	frame[INDEX(y, x, W * 3) + 0] = 255 - frame[INDEX(y, x, W * 3) + 0]; //Red
	frame[INDEX(y, x, W * 3) + 1] = 255 - frame[INDEX(y, x, W * 3) + 1]; //Green
	frame[INDEX(y, x, W * 3) + 2] = 255 - frame[INDEX(y, x, W * 3) + 2]; //Blue
      }
}

//2 boucles => 1 boucle
void process_one_loop(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  for (unsigned i = 0 ; i < index_max ; i++){
    //Invert each colour component of every pixel
    frame[i] = 255 - frame[i];
  }
}

//bitflip
void process_bitflip(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  for (unsigned i = 0 ; i < index_max ; i++){
    //Invert each colour component of every pixel
    frame[i] = ~frame[i];
  }
}

//unroll 8
void process_unroll_8(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  unsigned repeat = index_max >> 3; //divided by 8
  unsigned left = index_max % 8;
  unsigned i = 0;
  while(repeat--){
    //Invert each colour component of every pixel
    frame[i] = 255 - frame[i];
    frame[i + 1] = 255 - frame[i + 1];
    frame[i + 2] = 255 - frame[i + 2];
    frame[i + 3] = 255 - frame[i + 3];
    frame[i + 4] = 255 - frame[i + 4];
    frame[i + 5] = 255 - frame[i + 5];
    frame[i + 6] = 255 - frame[i + 6];
    frame[i + 7] = 255 - frame[i + 7];

    i+=8;
  }

  switch(left){
    case 7: frame[i + 6] = 255 - frame[i + 6];
    case 6: frame[i + 5] = 255 - frame[i + 5];
    case 5: frame[i + 4] = 255 - frame[i + 4];
    case 4: frame[i + 3] = 255 - frame[i + 3];
    case 3: frame[i + 2] = 255 - frame[i + 2];
    case 2: frame[i + 1] = 255 - frame[i + 1];
    case 1: frame[i] = 255 - frame[i];
    case 0:
    default: ;
  }
}

//unroll 8 bitflip
void process_unroll_8_bitflip(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  unsigned repeat = index_max >> 3; //divided by 8
  unsigned left = index_max % 8;
  unsigned i = 0;
  while(repeat--){
    //Invert each colour component of every pixel
    frame[i] = ~frame[i];
    frame[i + 1] = ~frame[i + 1];
    frame[i + 2] = ~frame[i + 2];
    frame[i + 3] = ~frame[i + 3];
    frame[i + 4] = ~frame[i + 4];
    frame[i + 5] = ~frame[i + 5];
    frame[i + 6] = ~frame[i + 6];
    frame[i + 7] = ~frame[i + 7];

    i+=8;
  }

  switch(left){
    case 7: frame[i + 6] = ~frame[i + 6];
    case 6: frame[i + 5] = ~frame[i + 5];
    case 5: frame[i + 4] = ~frame[i + 4];
    case 4: frame[i + 3] = ~frame[i + 3];
    case 3: frame[i + 2] = ~frame[i + 2];
    case 2: frame[i + 1] = ~frame[i + 1];
    case 1: frame[i] = ~frame[i];
    case 0:
    default: ;
  }
}

//parallel
void process_parallel(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel for schedule(static)
  for (unsigned i = 0 ; i < index_max ; i++){
    //Invert each colour component of every pixel
    frame[i] = 255 - frame[i];
  }
}

//parallel bitflip
void process_parallel_bitflip(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel for schedule(static)
  for (unsigned i = 0 ; i < index_max ; i++){
    //Invert each colour component of every pixel
    frame[i] = ~frame[i];
  }
}

//unroll 8 parallel
void process_unroll_8_parallel(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  unsigned repeat = index_max >> 3; //divided by 8
  unsigned left = index_max % 8;
  unsigned i = 0;
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel for private(i) schedule(static)
  for(unsigned j = 0 ; j < repeat ; j++){
    //Invert each colour component of every pixel
    i = j << 3; //multiply by 8
    frame[i] = 255 - frame[i];
    frame[i + 1] = 255 - frame[i + 1];
    frame[i + 2] = 255 - frame[i + 2];
    frame[i + 3] = 255 - frame[i + 3];
    frame[i + 4] = 255 - frame[i + 4];
    frame[i + 5] = 255 - frame[i + 5];
    frame[i + 6] = 255 - frame[i + 6];
    frame[i + 7] = 255 - frame[i + 7];
  }

  i = repeat << 3; //repeat * 8
  switch(left){
    case 7: frame[i + 6] = 255 - frame[i + 6];
    case 6: frame[i + 5] = 255 - frame[i + 5];
    case 5: frame[i + 4] = 255 - frame[i + 4];
    case 4: frame[i + 3] = 255 - frame[i + 3];
    case 3: frame[i + 2] = 255 - frame[i + 2];
    case 2: frame[i + 1] = 255 - frame[i + 1];
    case 1: frame[i] = 255 - frame[i];
    case 0:
    default: ;
  }
}

//unroll 8 + parallel bitflip
void process_unroll_8_parallel_bitflip(u8 *frame)
{
  unsigned W_max = W * 3;
  unsigned index_max = INDEX(H, W_max, W_max);
  unsigned repeat = index_max >> 3; //divided by 8
  unsigned left = index_max % 8;
  unsigned i = 0;
  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel for private(i) schedule(static)
  for(unsigned j = 0 ; j < repeat ; j++){
    //Invert each colour component of every pixel
    i = j << 3; //multiply by 8
    frame[i] = ~frame[i];
    frame[i + 1] = ~frame[i + 1];
    frame[i + 2] = ~frame[i + 2];
    frame[i + 3] = ~frame[i + 3];
    frame[i + 4] = ~frame[i + 4];
    frame[i + 5] = ~frame[i + 5];
    frame[i + 6] = ~frame[i + 6];
    frame[i + 7] = ~frame[i + 7];
  }

  i = repeat << 3; //repeat * 8
  switch(left){
    case 7: frame[i + 6] = ~frame[i + 6];
    case 6: frame[i + 5] = ~frame[i + 5];
    case 5: frame[i + 4] = ~frame[i + 4];
    case 4: frame[i + 3] = ~frame[i + 3];
    case 3: frame[i + 2] = ~frame[i + 2];
    case 2: frame[i + 1] = ~frame[i + 1];
    case 1: frame[i] = ~frame[i];
    case 0:
    default: ;
  }
}

//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [raw input file] [raw output file]\n", argv[0]), 1;
  
  //
  u64 cycles[MAX_SAMPLES], cycles_a, cycles_b;
  u64 nb_bytes = 1, frame_count = 0, samples_count = 0, frame_size = sizeof(u8) * H * W * 3;
  i32 i;
  
  //
  u8 *frame = _mm_malloc(frame_size, 32);

  //
  FILE *fpi = fopen(argv[1], "rb"); 
  FILE *fpo = fopen(argv[2], "wb");

  //
  if (!fpi)
    return printf("Error: cannot open file '%s'\n", argv[1]), 2;
  
  //
  if (!fpo)
    return printf("Error: cannot open file '%s'\n", argv[2]), 2;
  
  //Read & process video frames
  while ((nb_bytes = fread(frame, sizeof(u8), H * W * 3, fpi)))
    {
      if (nb_bytes < frame_size)
	return printf("Error: incorrect frame size\n"), 3;
      
      //Start 
      cycles_b = rdtsc();

      //Put other versions here
      
#if BASELINE
      process_baseline(frame);
#endif 
#if ONE_LOOP
      process_one_loop(frame);
#endif
#if BITFLIP
      process_bitflip(frame);
#endif
#if UNROLL_8
      process_unroll_8(frame);
#endif
#if UNROLL_8_BITFLIP
      process_unroll_8_bitflip(frame);
#endif
#if PARALLEL
      process_parallel(frame);
#endif   
#if PARALLEL_BITFLIP
      process_parallel_bitflip(frame);
#endif  
#if UNROLL_8_PARALLEL
      process_unroll_8_parallel(frame);
#endif 
#if UNROLL_8_PARALLEL_BITFLIP
      process_unroll_8_parallel_bitflip(frame);
#endif    
      //Stop
      cycles_a = rdtsc();

      //
      u64 elapsed = (cycles_a - cycles_b);

      //
      if (samples_count < MAX_SAMPLES)
	cycles[samples_count++] = elapsed;
      
      //frame number; size in Bytes; elapsed cycles; bytes per cycle
      fprintf(stdout, "%20llu; %20llu; %20llu; %15.3lf\n", frame_count, nb_bytes, elapsed, (f64)(nb_bytes) / (f64)elapsed);
      
      // Write this frame to the output pipe
      fwrite(frame, sizeof(u8), H * W * 3, fpo);

      //
      frame_count++;
    }

  //
  sort(cycles, samples_count);

  //
  f64 min, max, avg, mea, dev, bpc;

  //
  mea = mean(cycles, samples_count);
  
  //
  dev = stddev(cycles, samples_count);

  //
  min = cycles[0];
  max = cycles[samples_count - 1];
  avg = (min + max) / 2.0;

  //
  bpc = frame_size / mea;

  //
  fprintf(stderr, "\n%20llu; %15.3lf; %15.3lf; %15.3lf; %15.3lf; %15.3lf; %15.3lf %%;\n",
	  (sizeof(u8) * H * W * 3),
	  min,
	  max,
	  avg,
	  mea,
	  bpc,
	  (dev * 100.0 / mea));
  
  //
  _mm_free(frame);

  //
  fclose(fpi);
  fclose(fpo);

  return  0;
}
