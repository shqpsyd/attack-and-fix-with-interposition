#include<unordered_map>
#include<cassert>
#include<cstdio>
#include<cstring>
#include<map>
#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<iomanip>
#include<fcntl.h>
#include<sys/wait.h>

using namespace std;

vector<size_t> *malloc_data = NULL;
map<int ,int> *myhash1 = NULL; 
map<int, int> *myhash2 = NULL;

/* declare the following functions to have "C" linkage, so that
 * we can see them from C code without doing name demangling. */
extern "C"
{
	
	void so_deallocate()
	{
		delete malloc_data;
		delete myhash1;
		delete myhash2;
	}

	void so_allocate()
	{
		if(malloc_data == NULL)
		{
			malloc_data = new vector<size_t>();
		}
		if(myhash1 == NULL) {
			myhash1 = new map<int,int>();			
		}
		if(myhash2 == NULL) {
			myhash2 = new map<int,int>();			
		}
	}

    /* dump() - output the contents of the malloc_data */
    void malloc_dump()
	{
		for(auto& data : *malloc_data)
		{
			//printf("Allocations made: %ld.\n", data);
		}
	}

	/* insert() - when malloc() is called, the interpositioning library
	 *            stores the size of the request.	*/
	
	int malloc_insert(size_t size)
	{
		so_allocate();
		malloc_data->push_back(size);
	}

	void putMoney(int fname, int money) {
		//printf("%i",fname);
		(*myhash1)[fname] = money;
		
	}
	int getMoney(int fname) {
		return (*myhash1)[fname];
	}
	void putFname(int fd, int fname) {
		(*myhash2)[fd] = fname;
	}
	int getFname(int fd){
		
		return (*myhash2)[fd];
	}
}
