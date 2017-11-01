#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
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
	int counter = 0;
	

public:
	void getData(string path)
	{
		int n;
		
		vector <int> pom;
		ifstream plik;
		string line;
		plik.open(path, ios::in);
		while (!plik.eof())
		{
			getline(plik, line);
			if (line[0] == ';')
			{
				string tmp;
				int value;
				char c;
				stringstream ss(line);
				ss >> c >> tmp >> value;
				if (tmp == "MaxNodes:") maxNodes = value;
				else if (tmp == "MaxProcs:") maxProcs = value;
			}
			else
			{
				Task task;
				stringstream stream(line);
				while (stream >> n)
				{
					pom.push_back(n);
				}
				task.id = pom[0];
				task.submit = pom[1];
				task.run = pom[3];
				task.proc = pom[4];
				pom.clear();
				data.push_back(task);
				counter++;
			}

		}
		plik.close();
	}
	void showData()
	{
		cout << maxNodes << endl;
		cout << maxProcs << endl;
		for (int i = 0; i < counter; i++)
			cout << data[i].id << "\t" << data[i].submit << "\t" << data[i].run << "\t" << data[i].proc << endl;
	}

};

int main()
{
	Data tasklist;
	tasklist.getData("data1.txt");
	tasklist.showData();
	return EXIT_SUCCESS;
}