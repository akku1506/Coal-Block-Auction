#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

float totalTime;
long noOfBlocks;
long noOfBids;
long noOfCompanies;
float prob=0.5;
int* companyList = NULL;
int* blockList = NULL;
char* filename;

// denotes the Bid structure
typedef struct Bidding
{
	long companyId;
	long bidId;
	long amount;
	long noOfBlocks;
	long *blocks;
}bid;

bid** totalBids=NULL;

//denotes state
typedef struct structState
{
    long profit;
    long *blockAlloc;
}state; 

void initialise()
{
    companyList = (int *)malloc(noOfCompanies * sizeof(int));
    blockList = (int *)malloc(noOfBlocks * sizeof(int));
}

//initialising the entire bidlist
void initialiseBid(long companyId, long noOfBlocks, long amount, long *blocks, long bidId, bid** bidptr)
{   
    *bidptr=(bid *)malloc(sizeof(bid));
    (**bidptr).companyId=companyId;
    (**bidptr).bidId=bidId;
    (**bidptr).amount=amount;
    (**bidptr).noOfBlocks=noOfBlocks;
    (**bidptr).blocks=blocks;
}


//print to check for values
void print(bid** ptr)
{
 for (long k=0;k<noOfBids;k++)
    {
    printf("%ld Company Id ",ptr[k]->companyId);
    printf("\t %ld Bid Id ",ptr[k]->bidId);
    printf("\t %ld amount ",ptr[k]->amount);
    printf("\t %ld no",ptr[k]->noOfBlocks);
    for(int i=0;i<ptr[k]->noOfBlocks;i++)
    { 
        printf("\t blocks %ld  ",ptr[k]->blocks[i]);
    }
    printf ("\n");
    } 
}

void initialiseState(state** currState)
{
    //at a particular time state is all the bids assigned or not assigned
 	(*currState) = (state *) malloc(sizeof(state));
	(**currState).blockAlloc = (long *) calloc(noOfBids, sizeof(long));
	(**currState).profit = 0;
}

void freeState(state** currState)
{
	if((*currState) == NULL) 
        return;
	free((**currState).blockAlloc);
	free(*currState);
	(*currState) = NULL;
}

//check for consistent states and returns 1 if inconsistent
int checkInconsistentState(state* currState)
{
    int *companyList,*blockList;
    companyList=NULL;
    blockList=NULL;
    companyList=(int*) calloc(noOfCompanies,sizeof(int));
    blockList=(int*) calloc(noOfBlocks,sizeof(int));
    for(long i=0;i<noOfBids;i++)
    {
        if(currState->blockAlloc[i])
        {
            if(!companyList[(totalBids[i]->companyId)-1])
                companyList[(totalBids[i]->companyId)-1]=1;
            else
                {
                    free(companyList);
                    free (blockList);
                    return 1;
                }
            for (long j=0;j<totalBids[i]->noOfBlocks;j++)
            {
                if(!blockList[(totalBids[i]->blocks)[j]-1])
                    blockList[(totalBids[i]->blocks)[j]-1]=1;
                else 
                {
                    free(companyList);
                    free (blockList);
                    return 1;
                }
            }
        }
        
    }
    free(companyList);
    free (blockList);
    return 0;

}

void getConsistentState(state** currState,float timeDivision)
{
    struct timespec startTime,currTime;
    float diff=0;
    float timeConsistent;
    unsigned int number;    
    unsigned int max=RAND_MAX;
    int flag=0;    
    long* temp;  
    freeState(currState);
    initialiseState(currState);
    timeConsistent=timeDivision*0.5;
    
    //printf("profit value %ld \t",(*currState)->profit);
    clock_gettime(CLOCK_MONOTONIC,&startTime); 
    //printf("%lf",timeConsistent);
    while(diff<=timeConsistent)
    {
        temp=(*currState)->blockAlloc;
    //random assignment of 1's and 0's to an aaray
       for(long i=0;i<noOfBids;i++)
        {
            number=random();
            if (number>prob*max)
              *(temp+i)=1;
            else
                *(temp+i)=0;
        }
        if (!checkInconsistentState(*currState))
            {   
                flag=1;  
                break;
            }
        clock_gettime(CLOCK_MONOTONIC,&currTime); 
        diff=currTime.tv_sec-startTime.tv_sec;
    }
   // printf("flag is %d",flag);
    if (flag==0)
    {
        freeState(currState);
        initialiseState(currState);
        prob=prob/2;
    }

	for(long i=0;i<noOfBids;i++)
	{
		if((*currState)->blockAlloc[i])
			(*currState)->profit =(*currState)->profit + totalBids[i]->amount;
	}
    //resetting the values of companies and blocks that are still left
    memset((void *)companyList,0,sizeof(int)*noOfCompanies);
	memset((void *)blockList,0,sizeof(int)*noOfBlocks);
 	for(long i=0;i<noOfBids;i++)
	{	
        if(((*currState)->blockAlloc[i]))	
        {        
            companyList[totalBids[i]->companyId-1]=1;
			for(long j=0;j<totalBids[i]->noOfBlocks;j++)
			{
				blockList[(totalBids[i]->blocks)[j]-1]=1;
			}
        }
     }  
    //printf("value %ld \t",currState[0]->profit);
    //printf("value at %ld %ld %ld %ld %ld %ld",(currState[0]->blockAlloc[0]),(currState[0]->blockAlloc[1]),(currState[0]->blockAlloc[2]),(currState[0]->blockAlloc[3]),(currState[0]->blockAlloc[4]),(currState[0]->blockAlloc[5]));
    //printf("value at %d %d",companyList[0],companyList[1]);
    //printf("value at %ld %ld %ld %ld %ld %ld %ld",(blockList[0]),(blockList[1]),(blockList[2]),(blockList[3]),(blockList[4]),(blockList[5]),(blockList[6]));
}

void newNeighbors(state *currState,state ***returnList)
{
    int		isAllocatedHere = 1;	
	state	*newState = NULL;
	*returnList = (state **) malloc(sizeof(state*)*noOfBids);
	for(long i=0;i<noOfBids;i++)
	{
        initialiseState(&newState);    		
        memcpy(newState->blockAlloc,currState->blockAlloc,sizeof(long)*(noOfBids));

		if(currState->blockAlloc[i] == 0)
			newState->blockAlloc[i] = 1;
		else
			newState->blockAlloc[i] = 0;
        if(currState->blockAlloc[i]==1)
    
		    newState->profit = currState->profit-totalBids[i]->amount;
        else
		    newState->profit = currState->profit+totalBids[i]->amount;
		(*returnList)[i] = newState;
	}
}

int isNewConsistent(state *currentState, long newIndex)
{
    if(currentState->blockAlloc[newIndex] == 1)	
	{
	    bid	*newBid = totalBids[newIndex];
		if(companyList[newBid->companyId-1] == 0)
		{
			for(long i=0;i<newBid->noOfBlocks;i++)
				if(blockList[newBid->blocks[i]-1] == 1) 
                    return 0;
			return 1;
		}
		else
			return 0;

	}
	return 1;

}

void getNeighbours(state** newState,state* currState,float timeLeft,float startTime)
{
    struct timespec currTime;
    float diff=0;
    long	noOfNeighbors = noOfBids;
    state	**neighbours = NULL;
    while(currState)
    {
        freeState(newState);
        *newState=currState;
        clock_gettime(CLOCK_MONOTONIC, &currTime);
        diff=currTime.tv_sec-startTime;
        if(diff>=timeLeft)
            return;
        newNeighbors(currState,&neighbours);
	    state *nextState = currState;
	    long nextValue = (*currState).profit;
        long newIndex = 0;
        //number of bids as neighbours at particular point is either selcting or deselecting one bid to get new state
	    for(long i=0;i<noOfBids;i++)
	    {
		if((*(neighbours+i))->profit >= nextValue && isNewConsistent(*(neighbours+i), i))
		{
			if(nextState != (currState)) 
                freeState(&nextState);
			newIndex = i;
			nextState = neighbours[i];
			nextValue = nextState->profit;
		}
		else
			freeState(neighbours + i);
	    }
	    if(nextState == (currState))
		    (currState) = NULL;		
        else
        {
	        companyList[totalBids[newIndex]->companyId-1]=nextState->blockAlloc[newIndex];
	        for(long i = 0; i < totalBids[newIndex]->noOfBlocks; i++)
		        blockList[(totalBids[newIndex]->blocks)[i]-1]=nextState->blockAlloc[newIndex];
	        currState = nextState;
	    }
		free(neighbours);   
    }
}

void printConsistent(state *currState)
{
    //printf("consistent stsate");
    for(long i=0;i<noOfBids;i++)
        printf("%ld",currState->blockAlloc[i]);

}

void hillClimbing()
{
    struct timespec startTime,endTime,currTime;
    float timeLeft,timeDivision;
    state* resultState=NULL;
    //converting time in seconds
    totalTime=totalTime*60*0.95; 
    timeLeft=totalTime;
    // printf("%lf",totalTime);   
    srandom((unsigned int) time(NULL));
    clock_gettime(CLOCK_MONOTONIC,&startTime); 
    while(timeLeft>0)
    {    
        int iterations=10;
        //dividing time for random hill starts
        timeDivision=timeLeft/10;
        while(iterations>0)
        {             
           state* newState=NULL;           
           state* currState=NULL;
           clock_gettime(CLOCK_MONOTONIC,&currTime); 
           getConsistentState(&currState,timeDivision);
            //printf("commpany %ld %ld",companyList[0],companyList[1]);
            //printf("block %ld %ld %ld %ld %ld %ld %ld",(blockList[0]),(blockList[1]),(blockList[2]),(blockList[3]),(blockList[4]),(blockList[5]),(blockList[6]));
           getNeighbours(&newState,currState,timeDivision,currTime.tv_sec);//free(currState);
           //printf("\nvalue at %ld",newState->profit); ;
           if (resultState == NULL || (newState->profit > resultState->profit))
           {
                freeState(&resultState);
                resultState=newState;
                //printf("result profit %ld",resultState->profit);
           }
           else
            freeState(&newState);
           iterations=iterations-1; 
        }
        clock_gettime(CLOCK_MONOTONIC, &endTime);
        timeLeft = totalTime - (endTime.tv_sec-startTime.tv_sec);
        //printf("resultant %ld",(*resultState)->profit);
    }
    //writing back to file
	FILE *outputFile = fopen(filename, "w");
	fprintf(outputFile, "%ld", resultState->profit);
	for(long i = 0; i < noOfBids; i++)
	{
		if(resultState->blockAlloc[i])
		{
			fprintf(outputFile, " %ld", i);
		}
	}
	fclose(outputFile);
    freeState(&resultState);
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("\n Invalid arguments \n");
		exit(1);
	}

    long cid;
    long ncid;
    long noBlocks;
    long amount;
    int count=0;
    long* blockList=NULL;
    bid* bidptr=NULL;
    //outputfile
    filename=argv[2];
    // reading the file as passed in arguments
    FILE	*f1 = fopen(argv[1], "r");
    if(f1==NULL)
    {
        printf("\n No file found \n");
        exit(1);
    }
    fscanf(f1, "%f", &totalTime);
    fscanf(f1, "%ld", &noOfBlocks);
    fscanf(f1, "%ld", &noOfBids);
    fscanf(f1, "%ld", &noOfCompanies);
    //printf("%f \t %ld \t %ld \t %ld \t",totalTime,noOfBlocks,noOfBids,noOfCompanies);
    totalBids = (bid**)malloc(sizeof(bid *)*noOfBids);    
    for(long i=0;i<noOfCompanies;i++)
    {
        fscanf(f1, "%ld", &cid);
        fscanf(f1, "%ld", &ncid);
        for(long j=0;j<ncid;j++)
        {
            fscanf(f1, "%ld", &cid);
            fscanf(f1, "%ld", &noBlocks);
            fscanf(f1, "%ld", &amount);
            blockList=(long *) malloc(sizeof(long)*noBlocks);            
            for(long k=0;k<noBlocks;k++)
            {
                fscanf(f1, "%ld",  blockList+k);
            }

        initialiseBid(cid,noBlocks,amount,blockList,count,&bidptr);
        totalBids[count]=bidptr;
        count++;           
        }     
    }
    //print(totalBids);
    //initialise global variables
    initialise();
    hillClimbing();
	free(companyList);
	free(blockList);
}   
