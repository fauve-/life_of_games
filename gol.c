#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


const long offsetTop = 0x380000000;  //0x7 << 32;
const long offsetMid = 0x280000000; //0x5 << 32;
const long offsetBot = 0x380000000;// 0x7 << 32;
const char alive = '#';
const char dead = '.';

int  number_of_set_bits(int i){
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

unsigned int* turn(unsigned int* world, int side){
  printf("started turn\n");
  int writeSideOffset, readSideOffset;
  unsigned int i,j;
  writeSideOffset = side == 0 ? 32 : 0;
  readSideOffset = side == 0 ? 0 : 32;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 33; j++){
      int score = 0, mask = 0;
      mask = world[i + readSideOffset] & (offsetMid >> j);
      score += number_of_set_bits(mask);
      if(i != 0){
        mask = world[(i - 1) + readSideOffset] & (offsetTop >> j);
        score+=number_of_set_bits(mask);
      }
      if(i != 32){
        mask = world[i + 1 + readSideOffset] & (offsetBot >> j);
        score+=number_of_set_bits(mask);
      }
      printf("%i",score); //heatmap
      if((score == 3) ||
         (score == 2 && world[i + readSideOffset] & (1 << j))){
        world[i + writeSideOffset] |= (1 << j);
      }else{
        world[i + writeSideOffset] &= ~(1 << j);
      }
    }
    printf("\n"); //heatmap
  }
  printf("FRAME\n");
  return world;
}


void print(unsigned int *world, int side){
  int readOffset, i,j;
  char* outerString[32];
  readOffset = side == 0 ? 0 : 32;
  for(i = 0; i < 32; i++){
    char innerString[32];
    for(j = 0; j < 32; j++){
      innerString[j] = world[i + readOffset] & 1 << j ? alive : dead;
    }
    outerString[i] = (char *) malloc(sizeof(char) * 32);
    strncpy(outerString[i], innerString, 32);
  }
  for(i = 0; i < 32; i++){
    printf("%s\n", outerString[i]);
  }
}

unsigned int* populate_world(unsigned int *world){
  int i,j;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      if(rand() % 20 == 0){
        world[i] |= (1 << j);
      }
    }
  }
  return world;
}

void game_loop(unsigned int *world, int duration){
  int i;
  duration = duration == -1? ~0: duration;
  for(i = 0; i < duration;){
    world = turn(world, i % 2);
    print(world, ++i % 2);
  }
}

int main(){
  unsigned int *world;
  int i;
  srand(time(NULL));
  world = (unsigned int *) calloc(32 * 4, sizeof(unsigned int));
  printf("got world\n");
  world = populate_world(world);
  printf("populated\n");
  game_loop(world, 4);
  free(world);
  return 0;
}
