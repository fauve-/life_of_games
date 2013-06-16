#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//grabbed from the sweet stanford bithacks
//http://graphics.stanford.edu/~seander/bithacks.html

int  NumberOfSetBits(int i){
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#define GETWORLD(i) world[i]
const long offsetTop = 0x380000000;  //0x7 << 32;
const long offsetMid = 0x280000000; //0x5 << 32;
const long offsetBot = 0x380000000;// 0x7 << 32;

unsigned int * turn(unsigned int* world, int side){
  int othersideOffset, thisOffset;
  unsigned int i, j;
  //explicit for now, abstract away later
  thisOffset = side == 0 ? 0 : 32;
  othersideOffset = side == 0 ? 32: 0;
  for( i = 0; i < 32; i++){
    for(j = 1; j < 33; j++){
      int counter = 0;
      if(i != 0){
        int mask = (GETWORLD(i - 1 + thisOffset)) & (offsetTop >> j);
        counter+= NumberOfSetBits(mask);
      }
      if(1){//scoping
        int mask = GETWORLD(i + thisOffset) & (offsetMid >> j);
        counter+=NumberOfSetBits(mask);
      }
      if(i != 31){
        int mask = GETWORLD(i + 1 + thisOffset) & (offsetBot >> j);
        counter+=NumberOfSetBits(mask);
      }
      //printf("%i ",counter);
      if(counter == 3 || 
         ((counter == 2) && (GETWORLD(i + thisOffset) & (1 << j - 1 )))){
        (GETWORLD(i + othersideOffset)) |=  (1 << (32 - j));
      }else{
        (GETWORLD(i + othersideOffset)) &= ~(1 << (32 - j));
      }
    }
    //    printf("\n");
  }
  printf("FRAME\n");
  return world;
}

const char alive = '#';
const char dead = '.';

void printIt(unsigned int *world, int side){
  int i,j;
  //  char* outerstring[32];
  int thisOffset = side == 1? 32 : 0;
  for(i = 0; i < 32;i++){
    char innerstring[33];    
    for(j = 0; j < 32; j++){
      if(GETWORLD(i + thisOffset) & (1 << j)){
        innerstring[j] = alive;
      }else{
        innerstring[j] = dead;
      }
    }
    innerstring[32] = '\000';
    printf("%s\n",innerstring);
  }
}

unsigned int* seed_world(unsigned int *world, int life_chance){
  int i,j;
  for(i = 0; i < 32; i++){
    for(j = 0; j < 32; j++){
      if(rand() % (100 - life_chance) == 0){
        world[i] |= (1 << j);
      }
    }
  }
  return world;
}

unsigned int* seed_world_test(unsigned int *world){
  int i;
  for(i = 0; i < 32;i++){
    world[i] = 0xf0f0f0f0;
  }
  for(; i < 64; i++){
    world[i] = 0x0f0f0f0f;
  }
  return world;
}


void gameloop(unsigned int *world, int duration){
  int i;
  duration = duration == -1? ~0 : duration;
  printIt(world,0);
  for(i = 0; i < duration; i++){
    world = turn(world, i % 2);
    printIt(world, (i + 1) %2);
  }

}

int main(int argc, char** argv){
  unsigned int *world;
  int i, iterations, alive_percent;
  int *args;
  srand(time(NULL));  //  printf("%i \t %i",args[0], args[1]);
  gameloop(world,2);
  free(world);
  return 0;
}

