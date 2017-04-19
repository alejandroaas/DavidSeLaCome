#include "VirtualMemoryManager.h"
#include <stdio.h>
using namespace std;
//Test the Virtual memory manager here!!!!!!
int main(){
	int i;
	int ps = 2, nf = 2, va = 500;
	VirtualMemoryManager hello = VirtualMemoryManager(FIFO, ps, nf, va);
	cout << hello.memoryAccess(150);
	cin >> i;
	return 0;
}