#include "memoryManager.h"
#include <stdio.h>
using namespace std;
//Test the Virtual memory manager here!!!!!!
int main(){
	int i;
	unsigned int virtualAddressSize = -1, numFrames= -1, N= -1, policy=-1;
	unsigned long long address;
	bool go=true;
	cout << "Enter policy, LRU = 1, FIFO=0 :  ";
	cin >> policy;
	cout << policy << endl;
	cout << "Enter N in bits: ";
	cin >> N;
	cout << N << endl;
	cout << "Enter number of frames: ";
	cin >> numFrames;
	cout << numFrames << endl;
	cout << "Enter virtual address size: ";
	cin >> virtualAddressSize;
	cout << virtualAddressSize << endl;
	memoryManager mem = memoryManager( policy?LRU:FIFO , N, numFrames, virtualAddressSize);
	while (go) {
		cout << "Give an address: ";
		cin >> address;
		if (address == -1) break;
		cout << "Address: "<< address << endl;
		cout<<"Physical memory: " << mem.memoryAccess(address)<<endl;
		cout << "Swaps: " << mem.numberPageSwaps()<<endl<<endl<<endl;
	}
	return 0;
}