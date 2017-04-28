#include "memoryManager.h"
#include "virtualMemoryManagerInterface.hpp"
#include <map>
#include <vector>
#include <cmath>
/*
Team: Alejandro Andrade, David De Matheu, Kenneth Au
Constructor: Initializes variables
*/
memoryManager::memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) : virtualMemoryManagerInterface(p,  pS, nF, vA)
{
	//Base class inits important values...
	cycle = -1;//Init cycle
}
/*
Name:memoryAccess
Description: Implementation of virtual function memoryAccess. It access memory based on policy.
Input: virtual address
Processing: Gets physical address and updates pageTable and memory based on replacement policy.
Output: physical address
*/
unsigned long long memoryManager::memoryAccess(unsigned long long address){
	cycle++;// Increase cycle for LRU age and FIFO position.
	if (policy == 0){
		return FIFOPolicy(address);
	}
	else if (policy == 1){
		return LRUPolicy(address);
	}
	else
		return -1; //Error
}
/*
Function Name: LRUPolicy
Description: Implements LRUPolicy in memory.
Input: address --> virtual address
Processing: 1. Checks if virtual address in memory.
			2. If address not in memory, swap and put it on.
			3. Return physical address.
Output: Physical Address
*/
unsigned long long memoryManager::LRUPolicy(unsigned long long address){
	//Variables init
	unsigned long long temp;
	unsigned long long vpn = address >> N; // Virtual page number --> key of pageTable map
	cout <<"vpn : "<< vpn << endl;
	temp = vpn << N;
	unsigned long long offset = address - temp;
	cout << "offset: " << offset << endl;
	std::map<unsigned long long, page>::iterator pageTableIter, lru;
	pageTableIter = pageTable.find(vpn);
	//PageTable Check
	if (pageTableIter != pageTable.end()) { //Hit
		pageTableIter->second.status = cycle;
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}
	else if (pageTable.size() < numFrames) {//Compulsory miss
		cout << "Compulsory miss" << endl;
		pageTable[vpn].ppn = cycle; //Map pageTable into physical memory
		pageTable[vpn].status = cycle;
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}
	else {//Miss and Swap
		lru = findLRU();
		pageTable[vpn].ppn = lru->second.ppn;
		pageTable[vpn].status = cycle;
		pageTable.erase(lru->first);// Take away address from memory.
		cout << "Miss" << endl;
		numSwaps++;
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}

}
/*
Function Name: FifoPolicy
Description: Implements LRUPolicy in memory.
Input: address --> virtual address
Processing: 1. Checks if virtual address in memory.
			2. If address not in memory, swap and put it on.
			3. Return physical address.
Output: Physical Address
*/
unsigned long long memoryManager::FIFOPolicy(unsigned long long address){
	unsigned long long temp;
	unsigned long long vpn = address >> N; // Virtual page number --> key of pageTable map
	cout << "vpn : " << vpn << endl;
	temp = vpn << N;
	unsigned long long offset = address - temp;
	cout << "offset: " << offset << endl;
	std::map<unsigned long long, page>::iterator pageTableIter, fifo;
	pageTableIter = pageTable.find(vpn);
	//PageTable Check
	if (pageTableIter != pageTable.end()){ //Hit
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}
	else if (pageTable.size() < numFrames) {//Compulsory miss
		cout << "Compulsory miss" << endl;
		pageTable[vpn].ppn = cycle; //Map pageTable into physical memory
		pageTable[vpn].status = cycle;
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}
	else{//Miss and Swap
		fifo = findFIFO();
		pageTable[vpn].ppn = fifo->second.ppn;
		pageTable[vpn].status = cycle;
		pageTable.erase(fifo->first);// Take away address from memory.
		cout << "Miss" << endl;
		numSwaps++;
		return buildPhysicalAddress(pageTable[vpn].ppn, offset);
	}
}

/*
Name: buildPhysicalAddress
Description: Builds physical memory with ppn and offset
Input: 1. ppn
	   2. offset
	   3. Uses N as global var.
Procesing: Calculates physical memory by bit shifting N times to the left ppn,
		   and bit or the offset
Output: physical memory
*/
unsigned long long memoryManager::buildPhysicalAddress(unsigned long long ppn, unsigned long long offset){
	unsigned long long physicalAddress = -1;
	physicalAddress = ppn << (N/2);
	physicalAddress = physicalAddress | offset;
	return physicalAddress;

}

/*
Name: findLRU
Description: Finds memory with smallest status
Input: N/A. Uses class variables.
Procesing: Loop that checks for the smallest memory.
Output: Memory map iterator
*/
std::map<unsigned long long, page>::iterator memoryManager::findLRU(){
	std::map<unsigned long long, page>::iterator pageIter, lru;
	lru = pageTable.begin();
	for (pageIter = pageTable.begin(); pageIter != pageTable.end(); pageIter++) {
		if (lru->second.status > pageIter->second.status) {
			lru = pageIter;
		}
	}
	return lru;
}
/*
Name: findFIFO
Description: Finds memory with biggest status
Input: N/A. Uses class variables.
Procesing: Loop that checks for the smallest memory.
Output: Memory map iterator
*/
std::map<unsigned long long, page>::iterator memoryManager::findFIFO(){
	std::map<unsigned long long, page>::iterator pageIter, fifo;
	fifo = pageTable.begin();
	for (pageIter = pageTable.begin(); pageIter != pageTable.end(); pageIter++){
		if (fifo->second.status > pageIter->second.status){
			fifo = pageIter;
		}
	}
	return fifo;
}
memoryManager::~memoryManager()
{
	cout << "\nVirtual Memory Manager killed!\n";
}
