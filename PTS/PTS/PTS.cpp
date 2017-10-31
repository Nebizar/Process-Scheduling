#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Data {
private:
	vector<int> data_tab[4];
	//tab[0] - job number
	//tab[1] - r_j - submit time
	//tab[2] - r_j - run time
	//tab[3] - size_j - processors needed 

public:
	void getData(string path)
	{
		ifstream plik;
		plik.open(path, ios::in);

		plik.close();
	}

};

int main()
{

	return EXIT_SUCCESS;
}