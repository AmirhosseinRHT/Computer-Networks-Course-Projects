#include "senderGBN.hpp"

SenderGoBackN::SenderGoBackN(int _seqNum , int _totalFrames)
{
    seqNum = _seqNum;
    totalFrames = _totalFrames;
    currentIndex = 1;
    totalTransaction = 0;
}

void SenderGoBackN::updateIndex(std::vector<std::string> AckList)
{
    for(int j = 1 ;j < AckList.size();j++)
    {
        if(currentIndex == stoi(AckList[j]))
            currentIndex++;
        else
            break;
    }
}