#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Task {
	int id;//job number
	int submit;//r_j - submit time
	int run;//p_j - run time
	int proc;//size_j - processors needed 
};

class Data {
private:
	vector<Task> data;
	int maxNodes;
	int maxProcs;
	

public:
	void getData(string path)
	{
		ifstream plik;
		string line;
		plik.open(path, ios::in);
		while (!plik.eof())
		{
			getline(plik, line);

		}
		plik.close();
	}

};

int main()
{

	return EXIT_SUCCESS;
}