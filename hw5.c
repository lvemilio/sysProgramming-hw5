#include "header.h"

int chunkAmount = 0;
int allocatedChunks = 0;
int allocatedBytes = 0;
int totalBytes = 0;

int chunkSizes[256];
char* chunkPointerHead;
char* chunkPointerTail;
char* freeMemoryStart;

char* shared_memory = NULL;
char* shared_data = NULL;


/*Mode 0 =  BestFit
  Mode 1 = WorstFit
  Mode 2 = FirstFit
  Mode 3 = NextFit
  */
char* find_free_chunk(char* chunks,int bytesToAllocate, int mode){
    if(mode==0){
        int bestPointerIndex = 0;
        int bestSize = SHARED_MEMORY_SIZE;

        char* retVal;
        int i;
        for(i=0;i<chunkAmount;i++) {
            if(chunkSizes[i]==0) continue;
            int newSize = bytesToAllocate - chunkSizes[i];
            if(newSize<bestSize&&newSize>=0) {
                bestSize = chunkSizes[i];
                bestPointerIndex = i;
            }
        }
        if(bestSize==SHARED_MEMORY_SIZE) return NULL;
        retVal = &chunkPointerHead[bestPointerIndex];
        chunkSizes[bestPointerIndex] = 0;
        printf("Best Fit Chunk Size:%d\n",bestSize);
        return retVal;
    }
    else if(mode ==1){

    }
    else if(mode==2){

    }
    else if(mode==3){

    }
}


void startMemory(){
    shared_memory = mmap(NULL,SHARED_MEMORY_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    shared_data = (char*)(shared_memory +sizeof(int));
}


int getArguments(char* argv[],char* chunk, char* size){
    char* argument = (char* )malloc(sizeof(argv[1]));
    char* argument1= (char* )malloc(sizeof(argv[2]));

    argument = argv[1];
    argument1 = argv[2];

    if(strncmp(argument,CHUNK_CHECK,2)!=0){
        printf("First input paramater is incorrect.\n");
        return -1;
    }
    else if(strncmp(argument1,SIZE_CHECK,2)!=0){
        printf("Second input paramater is incorrect.\n");
        return -1;
    }
    int j;
    for(j=0;j<2;j++) strcpy(chunk,argument);
    for(j=0;j<2;j++) strcpy(size,argument1);

    
}

void allocateChunk(int* chunkValue){
    chunkSizes[chunkAmount] = *chunkValue;
    chunkAmount++;
}




int main(int argc, char* argv[]){
    char chunk[30];
    char size[30];
    
    int singleChunk;
    int bytesToAllocate;

    getArguments(argv,chunk,size);
    FILE* chunkFile;
    FILE* sizeFile;
    chunkFile = fopen("chunks1","r");
    sizeFile = fopen("sizes1","r");
    if(chunkFile==NULL||sizeFile==NULL)   return -1;
    
    startMemory();
    int i =0;

    fscanf(chunkFile,"%d",&singleChunk);
    while (!feof (chunkFile))
    {  
      allocateChunk(&singleChunk);
      totalBytes += singleChunk;
      fscanf (chunkFile, "%d", &singleChunk);      
    }

    int bookkeeping = SHARED_MEMORY_SIZE - totalBytes;

    chunkPointerHead = &shared_data[0];
    
    chunkPointerTail = &shared_data[chunkAmount];

    freeMemoryStart = &shared_data[bookkeeping];

    int j;
    for(j = 0;j<chunkAmount;j++){
        freeMemoryStart[allocatedBytes] = 'S';
        chunkPointerHead[j] = freeMemoryStart[allocatedBytes];
        allocatedBytes += chunkSizes[j];
    }
   for(j = 0;j<totalBytes;j++){
        if(freeMemoryStart[j]!=0) printf("%c",freeMemoryStart[j]);
        else printf("0");
    }
    /*
     fscanf(sizeFile,"%d",&bytesToAllocate);
    while (!feof (sizeFile))
    {  
      find_free_chunk(chunkPointerHead,bytesToAllocate,0);
      fscanf (sizeFile, "%d", &bytesToAllocate);      
    }*/
    
    return 0;

}