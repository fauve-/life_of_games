#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/*
  The overall plan is to keep each row in a 32-bit bitfield, and then
  run over each bit with a mask to check the neighbors , counting up
  the set bits to generate their score
  
  One interesting problem was to keep the board you're checking
  seperated from the screen you're writing on. To do that, I allocated
  twice as much memory for the board. 

  The print just iterates over the bits to check out if its set, and
  from that, decides what to lay down. It does that for the whole
  row, and then spits out that row.

  To generate the world, it's simply allocated, and then a random
  number modulous the (whole number) probability of life existing on
  that square. 
  
 */


//grabbed from the sweet stanford bithacks
//http://graphics.stanford.edu/~seander/bithacks.html

int  NumberOfSetBits(int i){
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

#define GETWORLD(i) world[i]

unsigned int * turn(unsigned int* world, int side){
  int othersideOffset, thisOffset;
  unsigned int i, j;
  //explicit for now, abstract away later
  thisOffset = side == 0? 0 : 32;
  othersideOffset = side == 0 ? 32: 0;
  const long offsetTop = 0x380000000;  //0x7 << 32;
  const long offsetMid = 0x280000000; //0x5 << 32;
  const long offsetBot = 0x380000000;// 0x7 << 32;
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
      //      printf("%i ",counter);
      if(counter == 3 || 
         ((counter == 2) && (GETWORLD(i + thisOffset) & (1 << j - 1 )))){
        (GETWORLD(i + othersideOffset)) |=  (1 << (j - 1));
      }else{
        (GETWORLD(i + othersideOffset)) &= ~(1 << (j - 1));
      }
    }
    //printf("\n");
  }
	printf("FRAME\n");
  return world;
}

const char alive = '#';
const char dead = '.';

void printIt(unsigned int *world, int side){
  int i,j;
  char* outerstring[32];
  int thisOffset = side == 1? 32 : 0;
  for(i = 0; i < 32;i++){
    char innerstring[32];    
    for(j = 0; j < 32; j++){
      if(GETWORLD(i + thisOffset) & (1 << j)){
        innerstring[j] = alive;
      }else{
        innerstring[j] = dead;
      }
    }
    outerstring[i] = (char*) malloc(sizeof(char) * 32);
    strncpy(outerstring[i], innerstring, 32);
  }
  for(i = 0; i < 32; i++){
    printf("%s\n", outerstring[i]);
  }
}

unsigned int* seed_world(unsigned int *world){
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
  for(i = 0; i < duration; i++){
    world = turn(world, i % 2);
    printIt(world, (i + 1) %2);
  }  
}


int main(){
  unsigned int *world;
  int i;
  //  turn(world, 1);
  srand(time(NULL));
  world = (unsigned int *) calloc(32 * 4,sizeof(unsigned int));
  world = seed_world(world);
  gameloop(world,1000000);
}

