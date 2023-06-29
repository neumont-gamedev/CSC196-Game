#include "Core/Random.h"
#include "Core/FileIO.h"
#include "Core/Memory.h"
#include <iostream>

using namespace std;

int main()
{
	int* p = new int;
	delete p;

	cout << kiko::getFilePath() << endl;
	kiko::setFilePath("Assets");
	cout << kiko::getFilePath() << endl;
	size_t size = 0;
	kiko::getFileSize("game.txt", size);
	cout << size << endl;

	std::string s;
	kiko::readFile("game.txt", s);
	cout << s << endl;


	kiko::seedRandom((unsigned int)time(nullptr));
	for (int i = 0; i < 3; i++)
	{
		cout << kiko::random(5, 10) << endl;
	}
}
