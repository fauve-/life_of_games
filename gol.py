#!/bin/python
#messing around with list comprehensions
#blowing out the carbon on python rusty-ness
#todo, implement toroidal landscape, or somesuch
import random
import time
#random helpers
def tupleSubtract(l1,l2):
    return (l1[0] + l2[0], l1[1] + l2[1])
def tupleOutOfBounds(l): 
    return l[0] < 0 or l[1] < 0 or l[0] >  31 or l[1] > 31

#state is a dict tuple(x,y):int
def loop(state):
    board = dict()
    offsets = [(i,j) for i in [0,-1, 1] for j in [0,-1,1] if (i,j) != (0,0)]
    #O(n^2 8)? jeeze
    for i in range(32):
        for j in range(32):
            neighborVals = []
            for of in offsets:
                poten = tupleSubtract((i,j), of)
                if not tupleOutOfBounds(poten):
                    neighborVals.append(state[poten])
            score = sum(neighborVals)
            if score == 3 or score == 2 and state[(i,j)] == 1:
                board[(i,j)] = 1
            else:
                board[(i,j)] = 0
    return board

#might switch to list comprehension for giggles.
def printIt(state):
    s = ""
    for i in range(32):
        for j in range(32):
            if state[(i,j)] == 1:
                s+= 'X'
            else:
                s+='.'
        s += "\n"
    print s
    time.sleep(1)#starts to get hard to follow otherwise

def init(x,y):
    board = dict()
    seeds = [1 if random.randint(1,100) % 20 == 0 else 0 for z in range(0,x*y)]
    coords = [(i,j) for i in range(x) for j in range(y)]
    for c in coords:
        board[c] = seeds.pop()
    return board

if __name__ == "__main__":
    b = init(32,32)
    while True:
        printIt(b)
        b = loop(b)

