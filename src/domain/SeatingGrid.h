#pragma once
#include <string>
#include <vector>

class SeatingGrid {
private:
    bool isSeatMapped;
    int rows;
    int cols;
    int maxCapacity;
    int bookedCount;
    std::vector<std::vector<bool>> seats; // true if occupied

public:
    // General capacity constructor
    SeatingGrid(int capacity);
    
    // Explicit seat mapping constructor
    SeatingGrid(int rows, int cols);

    bool isMapped() const;
    int getRows() const;
    int getCols() const;
    bool isSeatOccupied(int row, int col) const;
    int getAvailableCount() const;
    bool reserveGeneral(int count);
    bool reserveSeat(int row, int col);
    void releaseGeneral(int count);
    void releaseSeat(int row, int col);
    
    std::string serialize() const;
    static SeatingGrid deserialize(const std::string& rawData);
};