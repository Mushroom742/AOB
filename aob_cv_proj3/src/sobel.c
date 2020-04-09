/*
  This code performs edge detection using a Sobel filter on a video stream meant as input to a neural network
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <emmintrin.h>

//
#include "rdtsc.h"

//
#include "common.h"

//Convert an image to its grayscale equivalent - better color precision
void grayscale_weighted(u8 *frame)
{
  f32 gray;
  
  for (u64 i = 0; i < H * W * 3; i += 3)
    {
      //Convert RGB color values into grayscale for each pixel using color weights
      //Other possible weights: 0.59, 0.30, 0.11
      //Principle: mix different quantities of R, G, B to create a variant of gray
      gray = ((float)frame[i] * 0.299) + ((float)frame[i + 1] * 0.587) + ((float)frame[i + 2] * 0.114);
      
      frame[i]     = gray;
      frame[i + 1] = gray;
      frame[i + 2] = gray;
    }
}

//Convert an image to its grayscale equivalent - bad color precision
void grayscale_sampled(u8 *frame)
{
  for (u64 i = 0; i < H * W * 3; i += 3)
    {
      //R: light gray
      //G: medium gray
      //B: dark gray
      u8 gray = frame[i];
      
      frame[i]     = gray;
      frame[i + 1] = gray;
      frame[i + 2] = gray;
    }
}

//
i32 convolve_baseline(u8 *m, i32 *f, u64 fh, u64 fw)
{
  i32 r = 0;

  for (u64 i = 0; i < fh; i++)
    for (u64 j = 0; j < fw; j++)
      r += m[INDEX(i, j, fw)] * f[INDEX(i, j, fw)];
  
  return r;
}

//one loop
i32 convolve_one_loop(u8 *m, i32 *f, u64 fh, u64 fw)
{
  i32 r = 0;
  u64 index_max = INDEX(fh, 0, fw);

  for (u64 i = 0; i < index_max; i++)
    r += m[i] * f[i];
  
  return r;
}

//
void sobel_baseline(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  f32 mag = 0.0;

  i32 f1[9] = { -1, 0, 1,
		       -2, 0, 2,
		       -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
		       0, 0, 0,
		       1, 2, 1 }; //3x3 matrix
  
  //
  for (u64 i = 0; i < (H - 3); i++)
    for (u64 j = 0; j < ((W * 3) - 3); j++)
      {
      	gx = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f1, 3, 3);
      	gy = convolve_baseline(&cframe[INDEX(i, j, W * 3)], f2, 3, 3);
            
      	mag = sqrt((gx * gx) + (gy * gy));
      	
      	oframe[INDEX(i, j, W * 3)] = (mag > threshold) ? 255 : mag;
      }
}

//one loop
void sobel_one_loop(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  f32 mag = 0.0;

  i32 f1[9] = { -1, 0, 1,
           -2, 0, 2,
           -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
           0, 0, 0,
           1, 2, 1 }; //3x3 matrix

  u64 col_max = W * 3;
  u64 col_out = col_max - 3;
  u64 row_max = (H - 3);
  
  //
  for (u64 i = 0; i < row_max; i++){
    for (u64 j = i * col_max; (j % col_max) < col_out; j++)
      {
        gx = convolve_one_loop(&cframe[j], f1, 3, 3);
        gy = convolve_one_loop(&cframe[j], f2, 3, 3);

        mag = sqrt((gx * gx) + (gy * gy));
        
        oframe[j] = (mag > threshold) ? 255 : mag;
      }
  }
}

//without convolve
void sobel_without_convolve(u8 *cframe, u8 *oframe, f32 threshold)
{
  i32 gx, gy;
  i32 mag_square;
  i32 threshold_square = threshold * threshold;

  i32 f1[9] = { -1, 0, 1,
           -2, 0, 2,
           -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
           0, 0, 0,
           1, 2, 1 }; //3x3 matrix

  u64 col_max = W * 3;
  u64 col_out = col_max - 3;
  u64 row_max = (H - 3);
  
  //
  for (u64 i = 0; i < row_max; i++){
    for (u64 j = i * col_max; (j % col_max) < col_out; j++){
      gx = 0;
      gy = 0;

      for(u64 k = 0; k < 9; k++){
        u64 index = k + j;
        gx += cframe[index] * f1[k];
        gy += cframe[index] * f2[k];
      }

      mag_square = (gx * gx) + (gy * gy);
      
      oframe[j] = (mag_square > threshold_square) ? 255 : sqrt(mag_square);
    }
  }
}

static f32 sqrt_fast(f32 x)
 {
   u32 i = *(u32*) &x; 
   // adjust bias
   i  += 127 << 23;
   // approximation of square root
   i >>= 1; 
   return *(f32*) &i;
 }

//unroll 3rd loop
void sobel_unroll(u8 *cframe, u8 *oframe, f32 threshold)
{
  u64 index_max = (H - 3) * W * 3;
  i32 *gx = _mm_malloc(256 * sizeof(i32), 32);
  i32 *gy = _mm_malloc(256 * sizeof(i32), 32);
  f32 *mag = _mm_malloc(256 * sizeof(f32), 32);
  //i32 mag_square;
  //i32 threshold_square = threshold * threshold;

  /*i32 f1[9] = { -1, 0, 1,
           -2, 0, 2,
           -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
           0, 0, 0,
           1, 2, 1 }; //3x3 matrix*/

 // u64 col_max = W * 3;
  //u64 col_out = col_max - 3;
  //u64 row_max = (H - 3);
  
  u64 repeat = index_max >> 8;
  
  for(u64 j = 0; j < repeat; j++){
    u64 index = j << 8;
    for (u64 i = 0; i < 256; i++){

      gx[i] = 0;
      gy[i] = 0;

      gx[i] -= cframe[index];
      gy[i] -= cframe[index];
      gy[i] -= (cframe[index + 1] << 1);
      gx[i] += cframe[index + 2];
      gy[i] -= cframe[index + 2];
      gx[i] -= (cframe[index + 3] << 1);
      gx[i] += (cframe[index + 5] << 1);
      gx[i] -= cframe[index + 6];
      gy[i] += cframe[index + 6];
      gy[i] += (cframe[index + 7] << 1);
      gx[i] += cframe[index + 8];
      gy[i] += cframe[index + 8];

      index++;

    }

    for (u64 i = 0; i < 256; i++){
      gx[i] *= gx[i];
    }
    for (u64 i = 0; i < 256; i++){
      gy[i] *= gy[i];
    }
    for(u64 i = 0; i < 256; i++){
      gx[i] += gy[i]; //mag square
    }
    for(u64 i = 0; i < 256; i++){
      mag[i] = sqrt_fast(gx[i]); //mag
    }

    index = j << 8;
    for(u64 i = 0; i < 256; i++){

      oframe[index] = (mag[i] > threshold) ? 255 : mag[i];
      index++;
    }
  }
  
  _mm_free(gx);
  _mm_free(gy);
  _mm_free(mag);
}



void sobel_unroll_full(u8 *cframe, u8 *oframe, f32 threshold)
{
  u64 index_max = (H - 3) * W * 3;
  i32 *gx = _mm_malloc(index_max * sizeof(i32), 32);
  i32 *gy = _mm_malloc(index_max * sizeof(i32), 32);
  f32 *mag = _mm_malloc(index_max * sizeof(f32), 32);

  //i32 mag_square;
  //i32 threshold_square = threshold * threshold;

  /*i32 f1[9] = { -1, 0, 1,
           -2, 0, 2,
           -1, 0, 1 }; //3x3 matrix
  
  i32 f2[9] = { -1, -2, -1,
           0, 0, 0,
           1, 2, 1 }; //3x3 matrix*/

 // u64 col_max = W * 3;
  //u64 col_out = col_max - 3;
  //u64 row_max = (H - 3);
  
  //u64 repeat = index_max >> 8;
  for (u64 i = 0; i < index_max; i++){

    gx[i] = 0;
    gy[i] = 0;

    gx[i] -= cframe[i];
    gy[i] -= cframe[i];
    gy[i] -= (cframe[i + 1] << 1);
    gx[i] += cframe[i + 2];
    gy[i] -= cframe[i + 2];
    gx[i] -= (cframe[i + 3] << 1);
    gx[i] += (cframe[i + 5] << 1);
    gx[i] -= cframe[i + 6];
    gy[i] += cframe[i + 6];
    gy[i] += (cframe[i + 7] << 1);
    gx[i] += cframe[i + 8];
    gy[i] += cframe[i + 8];

  }

  for (u64 i = 0; i < index_max; i++){
    gx[i] *= gx[i];
  }
  for (u64 i = 0; i < index_max; i++){
    gy[i] *= gy[i];
  }
  for(u64 i = 0; i < index_max; i++){
    gx[i] += gy[i]; //mag square
  }
  for(u64 i = 0; i < index_max; i++){
    mag[i] = sqrt_fast(gx[i]); //mag
  }
  for(u64 i = 0; i < index_max; i++){

    oframe[i] = (mag[i] > threshold) ? 255 : mag[i];  }
  
  _mm_free(gx);
  _mm_free(gy);
  _mm_free(mag);
}



//
int main(int argc, char **argv)
{
  //
  if (argc < 3)
    return printf("Usage: %s [raw input file] [raw output file]\n", argv[0]), 1;
  
  //
  u64 size = sizeof(u8) * H * W * 3;
  u64 cycles[MAX_SAMPLES], cycles_a, cycles_b;
  u64 nb_bytes = 1, frame_count = 0, samples_count = 0;
 
  //
  u8 *cframe = _mm_malloc(size, 32);
  u8 *oframe = _mm_malloc(size, 32);

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
  while ((nb_bytes = fread(cframe, sizeof(u8), H * W * 3, fpi)))
    {
      //
      grayscale_weighted(cframe);
      
      //Start 
      cycles_b = rdtsc();

      //Put other versions here
      
#if BASELINE
      sobel_baseline(cframe, oframe, 100.0);
#endif
#if ONE_LOOP
      sobel_one_loop(cframe, oframe, 100.0);
#endif
#if WITHOUT_CONVOLVE
      sobel_without_convolve(cframe, oframe, 100.0);
#endif
#if UNROLL
      sobel_unroll(cframe, oframe, 100.0);
#endif
#if UNROLL_FULL
      sobel_unroll_full(cframe, oframe, 100.0);
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
      fwrite(oframe, sizeof(u8), H * W * 3, fpo);

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

  //Color frame, gray frame, output frame
  bpc = (size << 1) / mea;

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
  _mm_free(cframe);
  _mm_free(oframe);

  //
  fclose(fpi);
  fclose(fpo);

  return  0;
}
