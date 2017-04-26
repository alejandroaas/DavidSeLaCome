#include "memoryManager.h"
#include "virtualMemoryManagerInterface.hpp"
#include <map>
#include <vector>
#include <cmath>
/*
Constructor: Initializes variables
*/
memoryManager::memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) : virtualMemoryManagerInterface(p,  pS, nF, vA)
{
	//Base class inits important values...
	cycle = 0;//Init cycle
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
	unsigned long long vpn = address >> (N/2); // Virtual page number --> key of pageTable map
	unsigned long long ppn = -1; // Physical page number --> key of memory map 
	unsigned long long offset = address % vpn;
	std::map<unsigned long long, unsigned long long>::iterator pageTableIter;
	std::map<unsigned long long, frame>::iterator memoryIter, lru;
	pageTableIter = pageTable.find(vpn);
	//PageTable Check
	if (pageTableIter == pageTable.end()){//Compulsory miss
		pageTable[vpn] = cycle; //Map pageTable into physical memory
		ppn = cycle;
		memory[cycle] = buildFrame(offset);//Add frame to memory
		return buildPhysicalAddress(ppn, offset);
	}
	else{//Memory check
		memoryIter = memory.find(pageTableIter->second);
		ppn = pageTableIter->second;
		if (memoryIter->second.offset != offset){//MISS
			lru = findLRU();
			memory.erase(lru->first); // Take away address from memory.
			memory[ppn] = buildFrame(offset);
			numSwaps++;
			return buildPhysicalAddress(ppn, offset);
		}
		else{//HIT
			memory[ppn].status = cycle;  // Update age of frame
			return buildPhysicalAddress(ppn, offset);
		}

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
	//Variables init
	unsigned long long vpn = address >> (N/2); // Virtual page number --> key of pageTable map
	unsigned long long ppn = -1; // Physical page number --> key of memory map 
	unsigned long long offset = address % vpn;
	std::map<unsigned long long, unsigned long long>::iterator pageTableIter;
	std::map<unsigned long long, frame>::iterator memoryIter, fifo;
	pageTableIter = pageTable.find(vpn);

	//PageTable Check
	if (pageTableIter == pageTable.end()){//Compulsory miss
		pageTable[vpn] = cycle; //Map pageTable into physical memory
		ppn = cycle;
		memory[cycle] = buildFrame(offset);//Add frame to memory
		return buildPhysicalAddress(ppn, offset);
	}
	else{//Memory Check
		memoryIter = memory.find(pageTableIter->second);
		ppn = pageTableIter->second;
		if (memoryIter->second.offset != offset){//MISS
			fifo = findFIFO();
			memory.erase(fifo->first); // Take away address from memory.
			memory[ppn] = buildFrame(offset);
			numSwaps++;
			return buildPhysicalAddress(ppn, offset);
		}
		else{//HIT
			return buildPhysicalAddress(ppn, offset);
		}

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
Name: buildFrame
Description: Builds a frame with the ofset and cycle.
Input: 1. offset
	   2. Globaly references cycle
Procesing: assign offset and status of frame.
Output: frame
*/
frame memoryManager::buildFrame(unsigned long long offset){
	frame memFrame;//Frame for saving data
	memFrame.offset = offset;
	memFrame.status = cycle;
	return memFrame;

}
/*
Name: findLRU
Description: Finds memory with smallest status
Input: N/A. Uses class variables.
Procesing: Loop that checks for the smallest memory.
Output: Memory map iterator
*/
std::map<unsigned long long, frame>::iterator memoryManager::findLRU(){
	std::map<unsigned long long, frame>::iterator memoryIter, lru;
	lru = memory.begin();
	for (memoryIter = memory.begin(); memoryIter != memory.end(); memoryIter++){
		if (lru->second.status > memoryIter->second.status){
			lru = memoryIter;
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
std::map<unsigned long long, frame>::iterator memoryManager::findFIFO(){
	std::map<unsigned long long, frame>::iterator memoryIter, fifo;
	fifo = memory.begin();
	for (memoryIter = memory.begin(); memoryIter != memory.end(); memoryIter++){
		if (fifo->second.status > memoryIter->second.status){
			fifo = memoryIter;
		}
	}
	return fifo;
}
memoryManager::~memoryManager()
{
	cout << "Virtual Memory Manager killed!";
}
