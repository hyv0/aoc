#pragma once

class Dial{

public:
    Dial(int numberCount, int startNr);
	void turnLeft(int count);
	void turnRight(int count);
	int getCurrentNr() const;

private:
    int currentNr;
	int numberCount;

};
