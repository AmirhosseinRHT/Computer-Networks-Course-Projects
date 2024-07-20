#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class SenderGoBackN
{
public:
    SenderGoBackN(int _seqNum , int _totalFrames);
    void updateIndex(std::vector<std::string> AckList);
    int getTotalFrames() {return totalFrames;}
    int getSeqNum() {return seqNum;}
    int getTotalTransaction(){return totalTransaction;}
    int getCurrentIndex(){return currentIndex;}
    int increaseTotalTransaction() {totalTransaction++;}
    int increaseCurrentIndex() {currentIndex++;}
private:
    int totalFrames;
    int seqNum;
    int currentIndex;
    int totalTransaction;
};