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
	double ratio; //ratio = time*proc
};

struct Proc {
	int id;
	int start;
	int stop;
	vector <int> processors;
};

bool sortfunc2(Task i, Task j) { return (i.id<j.id); }

bool sortfunc(Task i, Task j) { return (i.submit<j.submit); }


double obliczSekundy(clock_t czas)
{
	return static_cast < double >(czas) / CLOCKS_PER_SEC;
}


class Data {
private:
	vector<Task> data;
	vector<Proc> output;
	int maxNodes; //==maxProcs
	int maxJobs;	//==maxRecords
	int data_size;
	int lowerBound;
	unsigned long long sumTime = 0;


public:

	void getData(string path, long quant)//get data from designated file
	{
		int n;
		data_size = 0;
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
				if (tmp == "MaxProcs:") maxNodes = value;
				else if (tmp == "MaxJobs:") maxJobs = value;
			}
			else if (line == "") continue;
			else
			{
				/*if (maxJobs < quant) {
				cout << "ERROR" << "\n";
				break;
				}*/
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
				task.ratio = task.run*task.proc;
				pom.clear();
				if (task.id != -1 && task.submit != -1 && task.run > 0 && task.proc != -1)
				{
					//cout << "go" << endl;
					data.push_back(task);
					data_size++;
					if (data_size == quant) break;
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
		for (int i = 0; i < data_size; i++)
			cout << data[i].id << "\t" << data[i].submit << "\t" << data[i].run << "\t" << data[i].proc << endl;
	}
	vector<Proc> const &getV() const {
		return output;
	}

	int getBound()
	{
		return lowerBound;
	}

	unsigned long long getSumTime()
	{
		return sumTime;
	}
	void schedule()
	{
		//Task* bestPerm;
		int *indeks;
		unsigned long long max = 0;
		maxJobs = data.size();
		unsigned long long time = 0;
		vector <Task> ready;
		int chosen;
		int readyS;
		bool choice;
		Proc out;
		unsigned long long* procTab = new unsigned long long[maxNodes];
		unsigned long long deltaT = 9999999999999999;
		//unsigned long long* procTab2 = new unsigned long long[maxNodes];
		for (int i = 0; i < maxNodes; i++) procTab[i] = 0;
		sort(data.begin(), data.end(), sortfunc);//sort by submit time
		int i = 0;
		int counter = 0;
		//cout << data[4].id << endl;
		while(i < maxJobs)//for each element
		{
			if (data[i].submit > time)
			{
				time = data[i].submit;
			}
			while (data[i].submit <= time)//if ask is ready - to vector
			{
				ready.push_back(data[i]);
				i++;
				if (i >= maxJobs) break;
			}
			//cout << "OK" << endl;
			/*for (int j = 0; j < ready.size(); j++)
			{
				cout << ready[j].id << endl;
			}
			cout << "-----" << endl;*/
			//WSTAWIANIE NA PROCESORY
			int check = 0;
			int counterH = 0;
			int orgS = ready.size();
			unsigned long long min,min2;
			while (counterH<orgS)
			{
				choice = false;
				readyS = ready.size();
				min = 999999999999999;
				for (int j = 0; j<maxNodes; j++)
				{
					if (procTab[j] <= time) check++;
				}
				for (int j = 0; j < readyS; j++)
				{
					if (ready[j].proc <= check && check - ready[j].proc < min)
					{
						min = check - ready[j].proc;
						chosen = j;
						//cout << j << endl;
						choice = true;
					}
				}
				//getchar();
				//cout<<check<<endl;
				if (choice)//if enough processors
				{
					//cout<<"good"<<endl;
					int finish = time + ready[chosen].run;
					counter = 0;
					for (int j = 0; j<maxNodes; j++)//insert finish time for every occupied processor
					{
						if (procTab[j] <= time)
						{
							procTab[j] = finish;
							out.processors.push_back(j);
							//cout<<j<<" ";
							counter++;
						}
						//cout<<counter<<endl;
						if (counter == ready[chosen].proc) break;
					}
					out.id = ready[chosen].id;//add to output
					out.start = time;
					out.stop = finish;
					output.push_back(out);
					ready.erase(ready.begin() + chosen );
					counterH++;
					//cout << "cH: " << counterH << endl;
					out.processors.clear();
				}
				else
				{
					min2 = 999999999999999;
					for (int j = 0; j < maxNodes; j++)
					{
						if (procTab[j]<min2 && procTab[j]>time)
							min2 = procTab[j];
					}
					time = min2;
					//cout << "Time: " << time << endl;
				}
				//cout<<endl;
				check = 0;
			}
			//KONIEC WSTAWNIANIA NA PROCKI
			if (obliczSekundy(clock()) >= 293)
			{
				cout << "Za duza instancja";
				break;
			}
			ready.clear();
		}

	}
};

int main(int argc, char **argv)
{
	Data tasklist;
	vector<Proc> best_output;
	char *p;
	long quant = strtol(argv[3], &p, 10);
	tasklist.getData(argv[1], quant);
	//tasklist.showData();
	ofstream plik;
	tasklist.schedule();
	best_output = tasklist.getV();
	plik.open(argv[2], ios::out);
	for (int i = 0; i < best_output.size(); i++)
	{
		plik << best_output[i].id << " " << best_output[i].start << " " << best_output[i].stop << " ";
		for (int j = 0; j < best_output[i].processors.size(); j++) plik << best_output[i].processors[j] << " ";
		plik << endl;
	}
	plik.close();
	return EXIT_SUCCESS;
}