#include "Dial.h"


Dial::Dial(int numberCount, int startNr){
    this->currentNr = startNr;
	this->numberCount = numberCount;

}


void Dial::turnLeft(int count){
    if(count > numberCount) {
        count = count % numberCount;
    }
    currentNr -= count;
    if(currentNr < 0) {
        currentNr += numberCount;
    }
}

void Dial::turnRight(int count){
    if(count > numberCount) {
        count = count % numberCount;
    }
    currentNr += count;
    if(currentNr >= numberCount) {
        currentNr -= numberCount;
    }
}

int Dial::getCurrentNr() const{
    return currentNr;
}