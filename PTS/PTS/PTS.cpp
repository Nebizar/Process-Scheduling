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

struct Proc {
	int id;
	int start;
	int stop;
	vector <int> processors;
};

class Data {
private:
	vector<Task> data;
	vector<Proc> output;
	int maxNodes; //==maxProcs
	int maxJobs;	//==maxRecords
	int counter = 0;

	vector <int> construction()//first greedy solution constructor
	{
		vector<int> solution;
		for (int i = 0; i < data.size(); i++)
		{

		}
	}
	

public:
	void getData(string path)//get data from designated file
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
				else if (tmp == "MaxJobs:") maxJobs = value;
			}
			else if (line == "") continue;
			else
			{
				Task task;
				stringstream stream(line);
				while (stream >> n)
				{
					pom.push_back(n);
				}
				//for (int i = 0; i < pom.size(); i++)
					//cout << pom[i] << "  ";              //Debug test
				//system("PAUSE");
				task.id = pom[0];
				task.submit = pom[1];
				task.run = pom[3];
				task.proc = pom[4];
				pom.clear();
				if (task.id != -1 && task.submit != -1 && task.run > 0 && task.proc != -1)
				{
					data.push_back(task);
					counter++;
				}
				else maxJobs--;
			}
		}
		plik.close();
	}
	void showData()//show needed data from file
	{
		cout << maxNodes << endl;
		cout << maxJobs << endl;
		for (int i = 0; i < counter; i++)
			cout << data[i].id << "\t" << data[i].submit << "\t" << data[i].run << "\t" << data[i].proc << endl;
	}
	void save(string path)//save output from algorithm in designated file
	{
		ofstream plik;
		plik.open(path, ios::out);
		for (int i = 0; i < output.size(); i++)
		{
			plik << output[i].id << " " << output[i].start << " " << output[i].stop << " ";
			for (int j = 0; j < output[i].processors.size(); j++) plik << output[i].processors[j] << " ";
			plik << endl;
		}
		plik.close();
	}

	void GRASP(int quant)//GRASP algorithm for finding solution
	{
		srand(time(NULL));
		int tSize = 0.1 * data.size();
		int *RCL = new int[tSize];
		vector <int> output;
		bool *procTab = new bool [maxNodes];



	}

};

int main()
{
	Data tasklist;
	tasklist.getData("data1.txt");
	//tasklist.showData();
	system("PAUSE");
	return EXIT_SUCCESS;
}