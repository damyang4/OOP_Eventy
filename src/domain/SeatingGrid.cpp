#include "SeatingGrid.h"
#include <sstream>

SeatingGrid::SeatingGrid(int capacity) 
    : isSeatMapped(false), rows(0), cols(0), maxCapacity(capacity), bookedCount(0) {}

SeatingGrid::SeatingGrid(int rows, int cols) 
    : isSeatMapped(true), rows(rows), cols(cols), maxCapacity(rows * cols), bookedCount(0) {
    seats.resize(rows, std::vector<bool>(cols, false));
}

bool SeatingGrid::isMapped() const { return isSeatMapped; }

int SeatingGrid::getAvailableCount() const { return maxCapacity - bookedCount; }

bool SeatingGrid::reserveGeneral(int count) {
    if (isSeatMapped || bookedCount + count > maxCapacity) return false;
    bookedCount += count;
    return true;
}

bool SeatingGrid::reserveSeat(int row, int col) {
    if (!isSeatMapped || row < 1 || row > rows || col < 1 || col > cols) return false;
    if (seats[row - 1][col - 1]) return false; // Already taken
    
    seats[row - 1][col - 1] = true;
    bookedCount++;
    return true;
}

void SeatingGrid::releaseGeneral(int count) {
    if (!isSeatMapped && bookedCount >= count) {
        bookedCount -= count;
    }
}

void SeatingGrid::releaseSeat(int row, int col) {
    if (isSeatMapped && row >= 1 && row <= rows && col >= 1 && col <= cols) {
        if (seats[row - 1][col - 1]) {
            seats[row - 1][col - 1] = false;
            bookedCount--;
        }
    }
}

std::string SeatingGrid::serialize() const {
    std::ostringstream ss;
    if (!isSeatMapped) {
        ss << "GEN:" << maxCapacity << ":" << bookedCount;
    } else {
        ss << "MAP:" << rows << ":" << cols << ":";
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                ss << (seats[r][c] ? "1" : "0");
            }
        }
    }
    return ss.str();
}

SeatingGrid SeatingGrid::deserialize(const std::string& rawData) {
    std::istringstream ss(rawData);
    std::string type;
    std::getline(ss, type, ':');
    
    if (type == "GEN") {
        std::string capStr, bookedStr;
        std::getline(ss, capStr, ':');
        std::getline(ss, bookedStr, ':');
        SeatingGrid grid(std::stoi(capStr));
        grid.reserveGeneral(std::stoi(bookedStr));
        return grid;
    } else {
        std::string rowsStr, colsStr, bits;
        std::getline(ss, rowsStr, ':');
        std::getline(ss, colsStr, ':');
        std::getline(ss, bits);
        
        int r = std::stoi(rowsStr);
        int c = std::stoi(colsStr);
        SeatingGrid grid(r, c);
        
        size_t idx = 0;
        for (int i = 1; i <= r; ++i) {
            for (int j = 1; j <= c; ++j) {
                if (idx < bits.size() && bits[idx] == '1') {
                    grid.reserveSeat(i, j);
                }
                idx++;
            }
        }
        return grid;
    }
}

int SeatingGrid::getRows() const {
    return rows;
}


int SeatingGrid::getCols() const {
    return cols;
}

bool SeatingGrid::isSeatOccupied(int row, int col) const {
    if (row < 1 || row > rows || col < 1 || col > cols) return false;
    return seats.at(row - 1).at(col - 1);
}