/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <bits/stdc++.h>
#define INPUT_BLOCK_SIZE 10
#define OUTPU_BLOCK_SIZE 500
#define OUTPU_BLOCK_MAX 5
#define ERROR_NULL_BLOCK 1
#define FAILED 2
#define SUCCESS 0

#define LOG(x,...) printf(": %d :" x, __LINE__, ##__VA_ARGS__) 

struct MemBlock {
    void  *block;
    int size;
    int filledSize;
    int lastPointerLocation;
} inputBlock[INPUT_BLOCK_SIZE];

struct MemBlock outputBlock[INPUT_BLOCK_SIZE];
static int outputBlockCount = 0;

void fillInputBlock(MemBlock *block, char * value, int length) {
    if (block == NULL || value == NULL) {
        LOG("Received unexpected null\n");
        return;
    }
    
    block->block = calloc(length, sizeof(char));
    if (!block->block) {
        LOG("Allocation failure\n");
    }
    
    block->size = length;
    block->lastPointerLocation = 0;
    block->filledSize = 0;
    memcpy(block->block, value, length);
}

void allocateOutputBlock(MemBlock *block, int length) {
    if (block == NULL) {
        LOG("Received unexpected null\n");
        return;
    }
    
    block->block = calloc(length, sizeof(char));
    if (!block->block) {
        LOG("Allocation failure\n");
    }
    
    block->size = length;
    block->lastPointerLocation = 0;
    block->filledSize = 0;
}

void printMemBlock(MemBlock *block) {
    int stride =0;
    if (block == NULL || block->block == NULL) {
        LOG("Received unexpected null\n");
        return;
    }
    LOG("Data @%p is %s\n",block->block, ((char *)block->block + stride));
}

int mergeWithOutput(MemBlock *outputBlock,MemBlock *inputBlock) {
    if (outputBlock == NULL || inputBlock == NULL) {
        LOG("Received unexpected null\n");
        return ERROR_NULL_BLOCK;
    }
    if (inputBlock->size <= 0 ){
        LOG("Received empty input block\n");
        return ERROR_NULL_BLOCK;
    }
    while (1) {
        if (outputBlock[outputBlockCount].size == 0) {
            allocateOutputBlock(&outputBlock[outputBlockCount],
                                OUTPU_BLOCK_SIZE);
        }
        int availableOutputSize = (outputBlock[outputBlockCount].size -
                                    outputBlock[outputBlockCount].filledSize);
        int remainingInputSize = (inputBlock->size - inputBlock->filledSize);
                            
        int sizeToBeCopied = availableOutputSize >= remainingInputSize ?
                             remainingInputSize : availableOutputSize;
                             
        memcpy(((char *)outputBlock[outputBlockCount].block + 
               outputBlock[outputBlockCount].lastPointerLocation),
               ((char *)inputBlock->block + inputBlock->lastPointerLocation),
               sizeToBeCopied);
               
        LOG("Houskeeping - Output filled %d LastPtr %d\n", outputBlock[outputBlockCount].filledSize,outputBlock[outputBlockCount].lastPointerLocation);
        LOG("Houskeeping - Input filled %d LastPtr %d\n", inputBlock->filledSize, inputBlock->lastPointerLocation);
        
        outputBlock[outputBlockCount].filledSize += sizeToBeCopied;
        inputBlock->filledSize += sizeToBeCopied;
        
        outputBlock[outputBlockCount].lastPointerLocation += sizeToBeCopied;
        inputBlock->lastPointerLocation += sizeToBeCopied;
        
        LOG("Houskeeping - Output filled %d LastPtr %d\n", outputBlock[outputBlockCount].filledSize,outputBlock[outputBlockCount].lastPointerLocation);
        LOG("Houskeeping - Input filled %d LastPtr %d\n", inputBlock->filledSize, inputBlock->lastPointerLocation);
        
        if (inputBlock->filledSize == inputBlock->size) {
            return SUCCESS;
        }

        if (outputBlockCount >= OUTPU_BLOCK_MAX) {
            LOG("Block allocation is full -  Info loss predicted\n");
            return FAILED;
        }                     
        if (outputBlock[outputBlockCount].size == 
            outputBlock[outputBlockCount].filledSize) {
            allocateOutputBlock(&outputBlock[++outputBlockCount],
                                OUTPU_BLOCK_SIZE);
        }
    }
    
    return FAILED;
    
}

void scatterGather(MemBlock *outputBlock,MemBlock *inputBlock,
                    int noOfInputBlock) {
    for(int i=0 ; i < noOfInputBlock; i++) {
        if (mergeWithOutput(outputBlock, &inputBlock[i])){
            LOG("scatterGather - failed block %d\n", i);
            break;
        }
    }
}

char TestStrings[][10] = {
    "Hello , ",
    "nightmare",
    " struck -",
    " with a",
    " hollow",
    " bullet"
};

int main()
{
    for(int i = 0 ; i < sizeof(TestStrings)/sizeof(TestStrings[0]) && i < INPUT_BLOCK_SIZE; i++) {
        fillInputBlock(&inputBlock[i], TestStrings[i], strlen(TestStrings[i]));
    }
    
    for(int i = 0 ; i < sizeof(TestStrings)/sizeof(TestStrings[0]) && i < INPUT_BLOCK_SIZE; i++) {
        //printMemBlock(&inputBlock[i]);
    }
    
    scatterGather(outputBlock, inputBlock, sizeof(TestStrings)/sizeof(TestStrings[0]));
    
    for(int i = 0 ; i <= outputBlockCount; i++) {
        LOG("printing %d\n", i);
        printMemBlock(&outputBlock[i]);
    }
    

    return 0;
}
