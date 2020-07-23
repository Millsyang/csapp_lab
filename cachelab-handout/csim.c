#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cachelab.h"

#define MACHINE_LEN 64

//an entry in cache table
typedef struct{
    bool valid;
    unsigned long tag;
    unsigned int lastUsedTime;
}Line;

//a cache table's parameter
typedef struct{
    int E;    //number of lines per set
    int s;    //number of index set bits
    int b;    //number of block bits
    Line **line;
}CacheTable;


//each record in trace file
typedef struct{
    char type;
    unsigned char NumOfBits;
    long address;  
}Record;

typedef struct _RecordNode{
    Record record;
    struct _RecordNode *next;
}RecordNode;


//simulate cache
void simulator(CacheTable,RecordNode *);
//search cache 
void searchCache(Line *line,unsigned long tag,char type);
//parse command line
bool parseCommandLine(int argc,char* const argv[],char* const optstring);
// init cache table
bool initCacheTable();
//parse trace file
bool parseTracefile(char* name);
//parse each record line in trace file
bool parseRecord(char* line,RecordNode *node);
//hex string to an integer var
long hex2long(char*,int);
//print help message
void printHelpMsg();
//just for test
void test();


/* Global Var*/
CacheTable cacheTable;
char filename[256];    //File which to be tested
RecordNode *recordList;      //Store all records
unsigned int clocktime = 0;   //Global time
int hits = 0;
int misses = 0;
int envictions = 0;


int main(int argc,char* argv[])
{
    char* const optstring = "hvs:E:b:t:";  //command line option
        
    if(!parseCommandLine(argc,argv,optstring)){  //parse command line
        printf("Parse command line failed!\n");
	return -1;
    }
    
    if(!initCacheTable()){      //init cache table
        printf("Init cache table failed!\n");
	return -1;
    }

    if(!parseTracefile(filename)){               //parse trace file
    	printf("Parse trace file failed!\n");
	return -1;
    }
    
    simulator(cacheTable,recordList);
    //test();
    printSummary(hits,misses,envictions);
    return 0;
}


//simulate cache
void simulator(CacheTable table,RecordNode *list){
    unsigned char t = MACHINE_LEN - (table.s + table.b);
    RecordNode* curNode = list;
    
    //traverse list
    while(curNode){
	clocktime++;
    	long address = curNode->record.address;
        unsigned int sNum = (address >> table.b) & ((1 << table.s) - 1);   //set index
	unsigned long tagNum = (address >> (MACHINE_LEN - t)) & ((1 << t) - 1);
    	//printf("sNum:%d tag:%ld.\n",sNum,tagNum);
	printf("%lx ",address);
	searchCache(*(table.line + sNum),tagNum,curNode->record.type);
	curNode = curNode->next;
    }
}


//search cache table
void searchCache(Line *line,unsigned long tag,char type){
    int i;
    int toAlterLine; 
    int length = cacheTable.E;
    unsigned int minTime = 0xffffffff;
    printf("%c %ld",type,tag);
    //traverse each line
    for(i = 0;i < length;i++){
        if(line[i].tag == tag && line[i].valid){
	    hits++;
	    line[i].lastUsedTime = clocktime;
	    printf("hit ");
	    break;
	}	
    }

    if(i == length){   //it means miss happened
        misses++;
	printf("miss ");
	
	//find a line to alter
	for(i = 0;i < length;i++){
	    if(!line[i].valid){    //not used,directly substitute
	    	toAlterLine = i;
		break;
	    }else if(line[i].lastUsedTime < minTime){
		minTime = line[i].lastUsedTime;
		toAlterLine = i;
	    }
	}

	if(line[toAlterLine].valid){
	    envictions++;
	    printf("envict ");
	}
	line[toAlterLine].valid = true;
	line[toAlterLine].tag = tag;
	line[toAlterLine].lastUsedTime = clocktime;
    }
    if(type == 'M'){
	hits++;
	printf("hit ");
    }
    printf("\n");
}


//parse command line
bool parseCommandLine(int argc,char* const argv[],char* const optstring){
    int optionchr;
    //opterr = 0;   //print error or not? 1:yes
    while((optionchr = getopt(argc,argv,optstring)) != -1){
        switch(optionchr){
	    case 'h':
		printHelpMsg();    //print help message
		break;
	    case 'v':
		printf("verbose option para\n");  //FIXME:here,we should print verbose run state
		break;
	    case 's':
		cacheTable.s = atoi(optarg);
		break;
	    case 'E':
		cacheTable.E = atoi(optarg);
		break;
	    case 'b':
		cacheTable.b = atoi(optarg);
	        break;
	    case 't':
		if(strlen(optarg) < 256)
	 	    strcpy(filename,optarg);
	    	else{
		    printf("Filename too long!\n");
		    return false;
		}
		break;
	    default:
		printf("Wrong option parameter!\n");
		return false;
	}
    }
    return true;
}


//init cache table
bool initCacheTable(){
    int S = 1 << cacheTable.s;
    
    //Allocate memory
    if(!(cacheTable.line = (Line **)malloc(sizeof(Line *) * S)))
	return false;
    for(int i = 0;i < S;i++){
        if(!(cacheTable.line[i] = (Line *)malloc(sizeof(Line) * cacheTable.E)))
	    return false;
    }
    
    //init table
    for(int grp = 0;grp < S;grp++){
        for(int idx = 0;idx < cacheTable.E;idx++){
	    cacheTable.line[grp][idx].valid = false;
	    cacheTable.line[grp][idx].tag = 0;
	    cacheTable.line[grp][idx].lastUsedTime = 0;
	}
    }

    return true;
}


//parse trace file
bool parseTracefile(char* name){
    FILE *tracefile; 
    char recordStr[0x10];
    
    if((tracefile = fopen(name,"r")) == NULL){
        printf("Open file:%s error!\n",name);
	return false;
    }

    RecordNode *curNode,*pre = NULL;
    if(!(curNode = (RecordNode *)malloc(sizeof(RecordNode))))
	return false;
    recordList = curNode;

    while(1){            //here,the code is ugly,need to be modified
        fgets(recordStr,0x10,tracefile);
	if(feof(tracefile)){
	    if(pre)
		pre->next = NULL;
	    free(curNode);
	    break;
	}
	if(!parseRecord(recordStr,curNode))
	    continue;
	if(!(curNode->next = malloc(sizeof(RecordNode))))  //here,if this two statements' order reversed,curNode may become nullptr;
	    return false;
	pre = curNode;
	curNode = curNode->next;
    }

    if(fclose(tracefile) != 0){
        printf("Close file:%s error!\n",name);
        return false;
    }
    return true;
}


//parse each record line in trace file
bool parseRecord(char* line,RecordNode *node){
    if(line[0] == 'I') //I starts means it is a instruction,just skip it
	return false;
    else{
        //create a new record node for global var recordList
	char addrStr[17];
	int i = 3;

	node->record.type = line[1];
	for(;line[i] != ',';i++){
	    addrStr[i - 3] = line[i];
	}
	addrStr[i - 3] = '\0';
        node->record.NumOfBits = line[i + 1];
	//memcpy(addrStr,line + 3,length-6);
	//addrStr[length - 6] = '\0';
    	node->record.address = hex2long(addrStr,i - 3);
	node->next = NULL;
	//printf("Address:%d,%p\n",node->record.address,node);
    }
    return true;
}
 
long hex2long(char* hexStr,int length){
    long ret = 0;
    int idx = 0;
    while(idx < length){
	char curNum = hexStr[idx];
        if(curNum >= 'a' && curNum <= 'f')
	    curNum -= 87;
	else
	    curNum -= 48;
	ret = (ret << 4) | curNum;
	idx++;
    }
    return ret;
}

//print help message
void printHelpMsg(){
    printf("Usage:./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Skipped......\n");
}

//just for test
void test(){
    printf("%d %d %d\n",cacheTable.s,cacheTable.E,cacheTable.b);
    printf("filename: %s\n",filename);

    //traverse recordList
    RecordNode *curNode = recordList;
    while(curNode){
    	printf("type:%c address:%ld size:%c.\n",curNode->record.type,curNode->record.address,curNode->record.NumOfBits);
	//printf("%p\n",curNode->next);
	curNode = curNode->next;
    }
}
