#include "header.h"

int chunkAmount = 0;
int allocatedChunks = 0;
int allocatedBytes = 0;
int totalBytes = 0;


int totalAllocatedMemory =0;
int chunkSizes[512];
char* chunkPointers[512];

char* freeMemoryStart;

char* shared_memory = NULL;
char* shared_data = NULL;


/*Mode 0 =  BestFit
  Mode 1 = WorstFit
  Mode 2 = FirstFit
  Mode 3 = NextFit
*/
char* find_free_chunk(char* chunks,int bytesToAllocate, int mode, int* adressIndex){
    if(mode==0){
        int bestPointerIndex = 0;
        int bestSize = SHARED_MEMORY_SIZE;

        int i;
        for(i=0;i<chunkAmount;i++) {
            if(chunkSizes[i]==0) continue;
            int newSize = chunkSizes[i] - bytesToAllocate ;
            if(newSize<bestSize&&newSize>=0) {
                bestSize = newSize;
                bestPointerIndex = i;
            }
        }
        if(bestSize==SHARED_MEMORY_SIZE) return NULL;
        *adressIndex = bestPointerIndex;
        return chunkPointers[bestPointerIndex];
    }
    else if(mode ==1){
        int bestPointerIndex = 0;
        int bestSize = 0;

        int i;
        for(i=0;i<chunkAmount;i++) {
            if(chunkSizes[i]==0) continue;
            int newSize = chunkSizes[i] - bytesToAllocate ;
            if(newSize>bestSize) {
                bestSize = newSize;
                bestPointerIndex = i;
            }
        }
        if(bestSize==0) return NULL;
        *adressIndex = bestPointerIndex;
        return chunkPointers[bestPointerIndex];
    }
    else if(mode==2){

    }
    else if(mode==3){

    }
}

void allocateMemory(char* memoryAdress, int bytesToAllocate, int adressIndex){

    int i = 1;
    
    printf("Adress index: %d\n",adressIndex);
    printf("Adress Size:%d\n",chunkSizes[adressIndex]);
    for(i=0;i<bytesToAllocate;i++) memoryAdress[i] = 'A';

    int bytesTilCurAdress = 0;
    int leftOverBytes = (chunkSizes[adressIndex]-bytesToAllocate);
    if(leftOverBytes>0){
        for(i=0;i<adressIndex;i++) bytesTilCurAdress+=chunkSizes[i];
        chunkPointers[chunkAmount] = &memoryAdress[bytesToAllocate];
        chunkSizes[chunkAmount] = leftOverBytes;
        chunkSizes[adressIndex]=0;
        chunkAmount++;
    }
}

void startMemory(){
    shared_memory = mmap(NULL,SHARED_MEMORY_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
    shared_data = (char*)(shared_memory +sizeof(char));
}


int getArguments(char* argv[],char* chunk, char* size){
    char* argument = (char* )malloc(sizeof(argv[1]));
    char* argument1= (char* )malloc(sizeof(argv[2]));

    strcpy(argument,argv[1]);
    strcpy(argument1,argv[2]);

    if(strncmp(argument,CHUNK_CHECK,2)!=0){
        printf("First input paramater is incorrect.\n");
        return -1;
    }
    else if(strncmp(argument1,SIZE_CHECK,2)!=0){
        printf("Second input paramater is incorrect.\n");
        return -1;
    }
    int j;
    for(j=0;j<2;j++) argument = strsep(&argv[1],"=");
    for(j=0;j<2;j++) argument1 = strsep(&argv[2],"=");

    strcpy(chunk,argument);
    strcpy(size,argument1);
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
    
    chunkFile = fopen(chunk,"r");
    sizeFile = fopen(size,"r");
    if(chunkFile==NULL||sizeFile==NULL)  return -1;
    
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

    freeMemoryStart = &shared_data[bookkeeping];

    int j;
    for(j = 0;j<chunkAmount;j++){
        freeMemoryStart[allocatedBytes] = 'S';
        chunkPointers[j] = &freeMemoryStart[allocatedBytes];
        allocatedBytes += chunkSizes[j];
    }
    
    fscanf(sizeFile,"%d",&bytesToAllocate);
    while (!feof (sizeFile))
    {  
      int adressIndex;
      char* memoryAdress;
      if((memoryAdress = find_free_chunk(freeMemoryStart,bytesToAllocate,0,&adressIndex))==NULL){
          printf("Could not allocate chunk with size %d\n",bytesToAllocate);
      }
      else{
          allocateMemory(memoryAdress,bytesToAllocate,adressIndex);
          totalAllocatedMemory+=bytesToAllocate;
      }
      fscanf (sizeFile, "%d", &bytesToAllocate);      
    }

    int aNumbers = 0;
    for(int i=0;i<totalBytes;i++){
        if(freeMemoryStart[i]==0)printf("0");
        else {
            if(freeMemoryStart[i]=='A')aNumbers++;
            printf("%c",freeMemoryStart[i]); }
    }

    printf("\nAllocated bytes in memory: %d\n",totalAllocatedMemory);
    
    return 0;

}