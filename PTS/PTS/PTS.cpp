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

bool sortfunc(Task i, Task j) { return (i.ratio<j.ratio); }


class Data {
private:
	vector<Task> data;
	vector<Proc> output;
	int maxNodes; //==maxProcs
	int maxJobs;	//==maxRecords
	int data_size;


	
	/*vector <int> construction()//first greedy solution constructor
	{
		vector<int> solution;
		for (int i = 0; i < data.size(); i++)
		{

		}
	}
	*/

public:
	void getData(string path, int quant)//get data from designated file
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
				if (tmp == "MaxNodes:") maxNodes = value;
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
	

	void GRASP(string path)//GRASP algorithm for finding solution
	{
		maxJobs = data.size();
		int WindowSize = 20;//Window - best elements
		int check;
		int selected;
		srand(time(NULL));
		unsigned int time = 0;
		//vector <int> RCL;
		vector <Proc> output;
		int *procTab = new int[maxNodes];//processors control free/busy and times
		for (int i = 0; i<maxNodes; i++)
		{
			procTab[i] = 0;
		}
		Proc *out = new Proc[maxJobs];
		int *window = new int[WindowSize];//table of best elements
		int *grasp = new int[maxJobs];//solution index only
		int counter = 0;
		sort(data.begin(), data.end(), sortfunc);//sort elements by ratio
		//cout << "check";
		for (int i = 0; i<WindowSize; i++)//throw to best elements by index
		{
			window[i] = counter;
			counter++;
		}
		int counter1 = 0;
		while (1)
		{
			check = 0;
			for (int j = 0; j<WindowSize; j++)//if empty in best elements, throw next best if there are processess
			{
				if (window[j] == -1)
				{
					if (counter<maxJobs)
					{
						window[j] = counter;
						counter++;
					}
					check++;
				}
			}
			if (check == WindowSize)//if only -1 break - all elements used
			{
				break;
			}
			selected = rand() % WindowSize + 0;//select random from window
			while (window[selected] == -1)//select another if -1
			{
				selected = rand() % WindowSize + 0;
			}
			grasp[counter1] = window[selected];//insert into solution
			counter1++;
			window[selected] = -1;//used element
		}
		//cout << "check";
		int counter_out = 0;
		while (1)
		{
			check = 0;
			for (int i = 0; i<maxJobs; i++)
			{
				if (grasp[i] != -1 && time >= data[grasp[i]].submit)
				{
					for (int j = 0; j<maxNodes; j++)
					{
						int counter2 = 0;
						if (procTab[j] <= time)//check if enough free processors next to one another
						{
							for (int k = j; k<j + data[grasp[i]].proc; k++)
							{
								if (procTab[k] <= time)
									counter2++;
							}
							if (counter2 == data[grasp[i]].proc)//if enough proc
							{
								int finish = time + data[grasp[i]].run;//end time
								for (int k = j; k<j + data[grasp[i]].proc; k++)//push back used processors
								{
									procTab[k] = finish;
									out[counter_out].processors.push_back(k);
								}
								out[counter_out].id = data[grasp[i]].id;
								out[counter_out].start = time;
								out[counter_out].stop = finish;
								output.push_back(out[counter_out]);
								//cout << "OK";
								counter_out++;
								grasp[i] = -1;
								break;
							}
						}
					}
				}
				else
				{
					if (grasp[i] == -1)
						check++;
				}
			}
			if (check == maxJobs)
			{
				break;
			}
			time++;
		}
		for (int i = 0; i<maxJobs; i++)
		{
			cout << "id " << output[i].id << " start " << output[i].start << " stop " << output[i].stop << " processors ";
			for (int n = 0; n<output[i].processors.size(); n++)
			{
				cout << output[i].processors[n] << " ";
			}
			cout << endl;
		}
		ofstream plik;
		plik.open("wyniki.txt", ios::out);
		for (int i = 0; i < data_size; i++)
		{
			plik << output[i].id << " " << output[i].start << " " << output[i].stop << " ";
			for (int j = 0; j < output[i].processors.size(); j++) plik << output[i].processors[j] << " ";
			plik << endl;
			//cout << i << " ";
		}
		plik.close();
	}

	void save(string path)//save output from algorithm in designated file
	{
		ofstream plik;
		plik.open(path, ios::out);
		for (int i = 0; i < data_size; i++)
		{
			plik << output[i].id << " " << output[i].start << " " << output[i].stop << " ";
			for (int j = 0; j < output[i].processors.size(); j++) plik << output[i].processors[j] << " ";
			plik << endl;
			cout << i << " ";
		}
		plik.close();
	}

};

int main()
{
	Data tasklist;
	tasklist.getData("DAS2-fs0-2003-1.swf", 500);
	//cout << "good";
	//tasklist.showData();
	tasklist.GRASP("wyniki.txt");
	//tasklist.save("wyniki.txt");
	system("PAUSE");
	return EXIT_SUCCESS;
}