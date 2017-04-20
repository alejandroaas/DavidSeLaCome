#include "VirtualMemoryManager.h"
#include <stdio.h>
using namespace std;
//Test the Virtual memory manager here!!!!!!
int main(){
	int i;
	int ps = 2, nf = 2, va = 500;
	VirtualMemoryManager hello = VirtualMemoryManager(LRU, ps, nf, va);
	cout << hello.memoryAccess(150);
	cout << hello.memoryAccess(151);
	cout << hello.numberPageSwaps();
	cin >> i;
	return 0;
}