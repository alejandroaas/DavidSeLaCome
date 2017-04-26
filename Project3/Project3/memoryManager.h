#pragma once
#include "virtualMemoryManagerInterface.hpp"
#include <map>
#include <vector>
#include <string>
struct frame
{
	unsigned long long offset;
	unsigned long long status=0; // Age or position
};

class memoryManager :
	public virtualMemoryManagerInterface
{
public:
	memoryManager::memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA);
	virtual unsigned long long memoryAccess(unsigned long long address);
	unsigned long long LRUPolicy(unsigned long long address);
	unsigned long long FIFOPolicy(unsigned long long address);
	virtual ~memoryManager();
private:
	map<unsigned long long, unsigned long long> pageTable; //Virtual memory
	map<unsigned long long, frame> memory; //Physical memory
	unsigned long long cycle; //Gives unique status.
protected:
	unsigned long long buildPhysicalAddress(unsigned long long ppn, unsigned long long offset);
	frame buildFrame(unsigned long long offset);
	std::map<unsigned long long, frame>::iterator findLRU();
	std::map<unsigned long long, frame>::iterator findFIFO();

};

