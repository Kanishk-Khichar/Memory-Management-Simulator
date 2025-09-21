#include <iostream>
#include <vector>
using namespace std;

struct Block {
    int start;      // Start address
    int size;       // Size of the block
    bool free;      // Is block free or allocated
};

class MemoryManager {
private:
    vector<Block> memory; // List of memory blocks
    int totalSize;

public:
    MemoryManager(int size) {
        totalSize = size;
        memory.push_back({0, size, true}); // Start with one big free block
    }

    // Display memory blocks
    void displayMemory() {
        cout << "\nMemory Blocks:\n";
        cout << "Start\tSize\tStatus\n";
        for (auto &b : memory) {
            cout << b.start << "\t" << b.size << "\t" << (b.free ? "Free" : "Allocated") << endl;
        }
    }

    // Allocate memory using best-fit or worst-fit
    void allocate(int size, string strategy) {
        int index = -1;

        if (strategy == "best") {
            int minSize = totalSize + 1;
            for (int i = 0; i < memory.size(); i++) {
                if (memory[i].free && memory[i].size >= size && memory[i].size < minSize) {
                    index = i;
                    minSize = memory[i].size;
                }
            }
        } else if (strategy == "worst") {
            int maxSize = -1;
            for (int i = 0; i < memory.size(); i++) {
                if (memory[i].free && memory[i].size >= size && memory[i].size > maxSize) {
                    index = i;
                    maxSize = memory[i].size;
                }
            }
        }

        if (index == -1) {
            cout << "No suitable block found for allocation of size " << size << endl;
            return;
        }

        // Allocate memory
        int startAddr = memory[index].start;
        memory[index].free = false;

        if (memory[index].size > size) {
            // Split block
            memory.insert(memory.begin() + index + 1, {startAddr + size, memory[index].size - size, true});
            memory[index].size = size;
        }

        cout << "Allocated " << size << " units at address " << startAddr << endl;
    }

    // Free memory at a given start address
    void deallocate(int startAddr) {
        int index = -1;
        for (int i = 0; i < memory.size(); i++) {
            if (memory[i].start == startAddr && !memory[i].free) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "No allocated block found at address " << startAddr << endl;
            return;
        }

        memory[index].free = true;
        cout << "Freed block starting at address " << startAddr << endl;

        // Merge adjacent free blocks
        if (index > 0 && memory[index - 1].free) {
            memory[index - 1].size += memory[index].size;
            memory.erase(memory.begin() + index);
            index--;
        }

        if (index < memory.size() - 1 && memory[index + 1].free) {
            memory[index].size += memory[index + 1].size;
            memory.erase(memory.begin() + index + 1);
        }
    }
};

int main() {
    int totalMemory = 100;
    MemoryManager mm(totalMemory);

    int choice, size, addr;
    string strategy;

    while (true) {
        cout << "\n=== Memory Management Simulator ===\n";
        cout << "1. Display Memory\n2. Allocate Memory\n3. Free Memory\n4. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                mm.displayMemory();
                break;
            case 2:
                cout << "Enter size to allocate: ";
                cin >> size;
                cout << "Enter strategy (best/worst): ";
                cin >> strategy;
                mm.allocate(size, strategy);
                break;
            case 3:
                cout << "Enter start address to free: ";
                cin >> addr;
                mm.deallocate(addr);
                break;
            case 4:
                exit(0);
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
