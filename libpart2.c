#include<stdint.h>
#include<stdio.h>
#include<dlfcn.h>
#include<string.h>
#include<errno.h>
#include<execinfo.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdarg.h>
//#include<stdlib.h>
/* We aren't providing much code here.  You'll need to implement quite a bit
 * for your library. */

/* Declarations for the functions in part2_hash.cc, so that we don't need an
 * extra header file. */
void malloc_insert(size_t size);
void malloc_dump();
void so_allocate();
void so_deallocate();
void putMoney(int fname, int money);
int getMoney(int fname);
void putFname(int fd, int  fname);
int getFname(int fd);
int transfer(char * name);
int atoi(const char *);
int strcmp(const char *str1, const char *str2);
int fscanf ( FILE * stream, const char * format, ...);
void *malloc(size_t bytes);



int open(const char *pathname, int flags,...);
ssize_t write(int fildes, const void *buf, size_t nbytes);
ssize_t read(int fildes, void *buf, size_t nbytes);


static int writeCount = 0;
static int fildes1 = 3;
static int fildes2 = 4;

static int option = -1;
static int ignoreMalloc = 0;
static int ignoreOpen = 0;
static int Tflag = 0;

static int M1 = 0;
static int M2 = 0;
static int count5 = 1;
static char* currentData1 = NULL;
static char* currentData2 = NULL;
static char* currentEnc1 = NULL;
static char* currentEnc2 = NULL;
static int count7 = 1;
static int fd = 3;
static int randomCount = 1;

int execvp(const char * path, char * const argv[]);
static char* options[100];
static int opNum = 0;



static char* allFiles[7] = {"bob.data","alice.data","rick.data","morty.data",
"picard.data","kirk.data","student.data"};

int execvp(const char * path, char * const argv[]) {
	if(option == 8) {
		//printf("path %s %s %s",path,argv[0],argv[1]);
		char*  op = malloc(10);
		strcpy(op,argv[1]);
		options[opNum++] = op;
		return 1;
	} else {
		int(*origExecvp)(const char*, char * const []) = NULL;
		origExecvp = (int(*)(const char*, char * const []))dlsym(RTLD_NEXT,"execvp");
		return origExecvp(path,argv);
	}	
}

int strcmp(const char *str1, const char *str2) {
    static int(*origStrcmp)(const char *, const char *) = NULL;
    if(!origStrcmp){
        origStrcmp = (int(*)(const char *str1, const char *str2))dlsym(RTLD_NEXT,"strcmp");

    }
    if(!origStrcmp) {
        fputs (dlerror(), stderr);
        return -1;
    }
    if(option == 1) {
  
        if(!origStrcmp(str1,"fakepwd")){
            printf("\nstudent's password is %s\n",str2);
            return 0;
        }        
    }
    return origStrcmp(str1,str2);
}
int fscanf ( FILE * stream, const char * format, ...)
{
    //printf("in fscanf");
    static int(*origFscanf)(FILE * , const char * ,...) = NULL;
    if(!origFscanf){
        origFscanf = (int(*)(FILE * , const char * ,...))dlsym(RTLD_NEXT,"fscanf");

    }
    if(!origFscanf) {
        fputs (dlerror(), stderr);
        return -1;
    }
    va_list ap;
    va_start(ap,format);
    char ** pwd = va_arg(ap,char**);
    
    if(!strcmp(format,"%ms")) {    
        //printf("here in");    
        *pwd = malloc(100);
        memset(*pwd,0,100);        
        strcpy(*pwd,"fakepwd");            
        return strlen(*pwd);       
    }
    return origFscanf(stream,format,pwd);
}




int atoi(const char * str) {
	static int (*origAtoi)(const char*) = NULL;
	if(!origAtoi) {
		origAtoi = (int (*)(const char *))dlsym(RTLD_NEXT,"atoi");
	}
	if(!origAtoi) {
		fputs (dlerror(), stderr);
		return -1;
	}
	
	return origAtoi(str);
}


//static char * (*itoa)(int , char * , int) = NULL;
int transfer(char * name) {
	int i = 0;
	for(; i < 7; i++) {
		if(!strcmp(name,allFiles[i])){
			return i;
		}
	}
}

int close(int fd){
	
	static int (*origClose)(int) = NULL;
	if(!origClose) {
		origClose = (int (*)(int))dlsym(RTLD_NEXT,"close");
	}
	if(!origClose) {
		fputs (dlerror(), stderr);
		return -1;
	}
	if(option == 3){
		putFname(fd,-1);
		printf("close fd[%i]\n",fd);
		return 0;
	}
	return origClose(fd);
	
}
int ftruncate(int fildes, off_t length) {
	static int (*origFtruncate)(int,off_t) = NULL;
	if(!origFtruncate){
		origFtruncate = (int (*)(int,off_t))dlsym(RTLD_NEXT,"ftruncate");
	}
	if(!origFtruncate) {
		fputs(dlerror(),stderr);
		return -1;
	}
	if(option == 3) {
		return 0;
	}
	return origFtruncate(fildes,length);
}
off_t lseek(int fildes, off_t offset, int whence) {
	static off_t (*origLseek)(int,off_t,int) = NULL;
	if(!origLseek){
		origLseek = (off_t (*)(int,off_t,int))dlsym(RTLD_NEXT,"lseek");
	}
	if(!origLseek) {
		fputs(dlerror(),stderr);
		return -1;
	}
	if(option == 3) {
		return 0;
	}
	return origLseek(fildes,offset,whence);
}

void *malloc(size_t bytes)
{
	static void* (*origMalloc)(size_t) = NULL;
	if(!origMalloc)
	{
		origMalloc = (void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");
	}

	if(ignoreMalloc)
	{
		return origMalloc(bytes);
	}

	ignoreMalloc = 1;
	malloc_insert(bytes);
	ignoreMalloc = 0;

	return origMalloc(bytes);
}

int open(const char *path, int oflag,...){
	
	static int (*origOpen)(const char *, int) = NULL;
	if (!origOpen) {
		origOpen = (int (*)(const char *, int))dlsym(RTLD_NEXT,"open");		
	}
	if(!origOpen){
		fputs (dlerror(), stderr);
		return -1;
	}
	
	if(atoi(path)){		
		option = atoi(path);
		return 	origOpen(path,oflag);				
	}
	
	if(option == 7) {
		
		char* currentData = malloc(20);	
		memset(currentData,0,20);		
		char* currentEnc = malloc(20);
		memset(currentEnc,0,20);
		char* suffix = ".enc";
		char*p = malloc(20);
		if(count7 == 1){
			memset(currentData1,0,20);
			strcpy(currentData1,path);
			memset(currentEnc1,0,20);
			strcpy(p,path);
			strcpy(currentEnc1,strtok(p,"."));			
			strcat(currentEnc1,suffix);
			strcpy(currentData,currentData1);
			strcpy(currentEnc,currentEnc1);
			count7 = 0;			
		}else{
			memset(currentData2,0,20);
			strcpy(currentData2,path);
			memset(currentEnc2,0,20);
			strcpy(p,path);
			strcpy(currentEnc2,strtok(p, "."));			
			strcat(currentEnc2,suffix);
			strcpy(currentData,currentData2);
			strcpy(currentEnc,currentEnc2);
			count7 = 1;	
		}
		int fenc = origOpen(currentEnc,0x0000);
		if(fenc == -1) {			
			char* data = malloc(20);			
			option = -1;
			int fdata = open(currentData,0x0000);
			
			read(fdata,data,20);			
			close(fdata);			
			printf("original data of %s is %s\n",currentData,data);
			FILE * f = fopen(currentEnc,"w+");
			for(int i = 0; i < strlen(data); i++) {
				data[i] = data[i]^currentData[i];
				
			}
			printf("%s's first data %s: xor with %s\n",currentEnc,data,currentData);
			//write(fenc,data,strlen(data));			
			fwrite(data,1,strlen(data),f);
			fclose(f);
			fenc = 	open(currentEnc,oflag);			
			option = 7;				
			return 	fenc;				
		}else {			
			close(fenc);			
			fenc = origOpen(currentEnc,oflag);
			return fenc;
		}

		
	}
	if(option == 3) {
		int i = 0;
		for( ; i < 7; i++) {
			if(!strcmp(allFiles[i],path)) {
				break;
			}
		}
		for(int i = 3; i < 100; i++) {
			if(getFname(i) == -1) {
				fd = i;
				break;
			}
		}		
		putFname(fd,transfer(allFiles[i]));
		printf("fd = %i, put %s into map\n",fd,allFiles[i]);
		return fd;		
	}
	
	return origOpen(path,oflag);
}
long int random () {
	
	static long int(*origRand)() = NULL;
	if(!origRand) {
		origRand = (long int(*)())dlsym(RTLD_NEXT,"random");
	}
	if(!origRand) {
		fputs (dlerror(), stderr);
		return -1;
	}
	if(option == 5){		
		if(count5 == 1) {
			count5 = 0;
			char Money[20];
			memset(Money,0,20);		
			
			if(randomCount == 7) {
				return 0;
			}						
			int f = open(allFiles[randomCount],0x0000);
			read(f,Money,20);
			close(f);
			int money = atoi(Money);
			if(money <10) {
				randomCount++;
				if(randomCount == 7) {
					return 0;
				}
				return randomCount;				
			}else{
				return randomCount;
			}

									
		} else {
			printf("return 7\n");
			count5 = 1;
			return 7;
			
		}
	}
	return origRand();
}
ssize_t read(int fildes, void *buf, size_t nbytes) {
	static ssize_t(*origRead)(int,void*,size_t) = NULL;
	if(origRead == NULL) {
		origRead = (ssize_t (*)(int, void*,size_t))dlsym(RTLD_NEXT,"read");
	}
	if(option == 4) {		
		
		if(fildes == fildes2) {
			M2 = atoi(buf);
		}
		return origRead(fildes,buf,nbytes);
	}
	if(option == 7) {
		char* data = malloc(20);
		memset(data,0,20);
		int result = origRead(fildes,data,nbytes);	
		printf("data read from .enc is %s\n",data);	
		if(fildes == fildes1) {		
			for (int i = 0; i < strlen(data); i++) {
								
				data[i] = data[i] ^ currentData1[i];
				
			}
			printf("data after xor with %s is %s\n",currentData1,data);
		}
		if(fildes == fildes2) {
			for (int i = 0; i < strlen(data); i++) {
				
				data[i] = data[i] ^ currentData2[i];
				
			}
			printf("data after xor with %s is %s\n",currentData2,data);
		}
		strcpy(buf,data);
		return result;	
	}
	if(option == 3) {
		int fname = getFname(fildes);
		int money = getMoney(fname);
		printf("read fildes = %i, fname = %i money = %i\n",fildes,fname,money);		
		char *Money = malloc(20);
		sprintf(Money,"%d",money);
		strcpy(buf,Money);
		
		return strlen(buf);
	}
	return origRead(fildes,buf,nbytes);
}
ssize_t write(int fildes, const void *buf, size_t nbytes) {
	//printf("fildes %i, buf %s, option %i\n",fildes,(char*)buf,option);
	static ssize_t(*origWrite)(int,const void*,size_t) = NULL;
	if(origWrite == NULL) {
		origWrite = (ssize_t (*)(int, const void*,size_t))dlsym(RTLD_NEXT,"write");
	}
	
	if(option == 2) {
		int money;
		if (fildes1 == fildes) {
			money = atoi(buf) - 200;
			printf("Bob's actual money %i\n",money);
		}
		if (fildes2 == fildes) {
			money = atoi(buf) + 200;
			printf("Alice's actual money %i\n",money);
		}
		char *Money = malloc(20);
		memset(Money,0,20);
		sprintf(Money,"%d",money);
		//printf("%s",Money);
		return origWrite(fildes,Money,strlen(Money));
	}
	if(option == 4) {		
		if(fildes2 == fildes) {
			if(atoi(buf)<=0) {
				return origWrite(fildes,buf,nbytes);
			}
			
			int umoney = atoi(buf) - 1;
			char *UMoney = malloc(20);
			memset(UMoney,0,20);
			sprintf(UMoney,"%d",umoney);
			int hecker = open("hacker.data",0x0002);
			char * buf2 = malloc(20);
			read(hecker,buf2,20);
			ftruncate(hecker,0);
			lseek(hecker,0,SEEK_SET);
			int hmoney = atoi(buf2) + 1;
			memset(buf2,0,20);
			sprintf(buf2,"%d",hmoney);
			//printf("hecker's money %s\n",buf2);			
			origWrite(hecker,buf2,strlen(buf2));
			close(hecker);
			option = 4;
			return origWrite(fildes,UMoney,strlen(UMoney));

		}
	}
	if(option == 7) {
		char * data = malloc(20);
		memset(data,0,20);
		//printf("original data need to write is %s",data);
		if(fildes == fildes1) {
			for(int i = 0; i < strlen(buf); i++) {
				
				data[i] =*(char*)(buf+i) ^ currentData1[i];
			}			
		}
		if(fildes == fildes2) {
			for(int i = 0; i < strlen(buf); i++) {
				data[i] = *(char*)(buf+i) ^ currentData2[i];
			}
		}
		//printf("data need to write after xor is %s",data);
		return origWrite(fildes,data,strlen(data));
	}
	if(option == 3) {
		int fname = getFname(fildes);
		putMoney(fname,atoi(buf));
		printf("write to %s, money = %i",allFiles[fname],atoi(buf));
		return nbytes;
		
	}
	if(option == 6) {
		int money = *((int*)buf);
		if(money == 0 && nbytes == 4) {
			writeCount = 0;
			writeCount++;
			return origWrite(fildes,buf,nbytes);
		}
		if(writeCount == 1 && nbytes == 4) {
			if(money > 0) {
				writeCount++;
				int i = 0;
				origWrite(18,buf,nbytes);
				return origWrite(fildes,&i,nbytes);
			}
			return origWrite(fildes,buf,nbytes);					
		}	
		return origWrite(fildes,buf,nbytes);
	}
	return origWrite(fildes,buf,nbytes);
}
__attribute__((constructor(101)))
static void allocate()
{
	so_allocate();
	printf("first cons");
}
__attribute__((constructor(102)))
static void writeMoney2H()
{
	
	currentData1 = malloc(20);
	currentData2 = malloc(20);
	currentEnc1 = malloc(20);
	currentEnc2 = malloc(20);
	printf("second cons\n");	
	
}
__attribute__((constructor(103)))
static void	readAll()
{	int temp = option;
	option = -1;
	
	for(int i = 0; i < 7; i++) {
		
		int f = open(allFiles[i],0x0000);
				
		char* buf = malloc(20);
		memset(buf,0,20);
		read(f,buf,20);	
		printf("buf = %s ",buf);	
		int money = atoi(buf);
		
		int fname = transfer(allFiles[i]);
		printf("%s %i %i %i\n",allFiles[i],f,money,fname);
		putMoney(fname, money);	
		money = getMoney(fname);
		printf("get money from fname%i %i\n",fname,money);	
		close(f);			
	}
	for(int i = 3; i<20; i++) {
		putFname(i,-1);
	}
	option = temp;
	printf("third cons\n");
}
__attribute__((destructor(101)))
static void updateAll() {
	if(option == 3){
	for(int i = 0; i < 7; i++) {
		option = -1;		
		int money = getMoney(transfer(allFiles[i]));
		printf("money from %s is %i\n", allFiles[i],money);
		int f = open(allFiles[i],0x0002);
		ftruncate(f,0);
		lseek(f,0,SEEK_SET);
		char Money[20];
		memset(Money,0,20);
		sprintf(Money,"%d",money);
		write(f,Money,strlen(Money));
	}
	
	printf("i'm out");
	}
}
__attribute__((destructor(102)))
static void execution(){
	printf("start exe\n");
	char* path = "./banker";
	for(int i = 0; i < opNum;i++) {
		option = -1;
		char * argv[3];
		argv[0] = "./banker" ;
		argv[1] = options[i];
		argv[2] = NULL;
		execvp(path,argv);
		printf("%s\n",argv[1]);
		for(int i = 0; i < 100000;i++) {			
			for(int j = 0; j < 100000;j++){
				for(int k = 0; k < 100000;k++);
			}
		}
		
	}

}
__attribute__((destructor(103)))
static void deallocate()
{
	malloc_dump();
	so_deallocate();
}







