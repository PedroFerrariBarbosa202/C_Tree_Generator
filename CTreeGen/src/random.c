#include <stdlib.h>
#include <time.h>

void init_rand_seed(){
  srand(time(NULL)); 
}

int get_random_1_0_m1(){
  float r = (float)rand() / RAND_MAX;

  if (r < 0.33) return -1;
  else if (r < 0.66) return 0;
  else return 1;
}

int get_random_1_m1(){
  float r = (float)rand() / RAND_MAX;

  if (r < 0.5) return -1;
  else return 1;
}

int get_random_range(int range){
  return (int)((double)rand() / (RAND_MAX + 1.0) * range);
}

float f_get_random_range(float range){
  return ((float)rand() / RAND_MAX) * range;
}
