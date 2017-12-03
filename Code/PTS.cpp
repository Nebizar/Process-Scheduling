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

struct Grasp {
	int index;
	int submit;
};

bool sortfunc(Task i, Task j) { return (i.ratio<j.ratio); }

bool sortfuncgrasp(Grasp i, Grasp j) { return (i.submit<j.submit); }


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
	unsigned int sumTime = 0;


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


	unsigned long long GRASP()//GRASP algorithm for finding solution
	{
		sumTime = 0;
		maxJobs = data.size();
		int WindowSize;
		if (maxJobs == 1)
			WindowSize = 1;//Window - best elements
		else if (maxJobs <= 20) WindowSize = 2;
		else WindowSize = 20;
		int check;
		int selected;
		srand(time(NULL));
		unsigned long long int time = 0;
		//vector <int> RCL;
		int *procTab = new int[maxNodes];//processors control free/busy and times
		for (int i = 0; i<maxNodes; i++)
		{
			procTab[i] = 0;
		}
		Proc *out = new Proc[maxJobs];
		int *window = new int[WindowSize];//table of best elements
		Grasp *grasp = new Grasp[maxJobs];//solution index only
		int counter = 0;
		int counter_out = 0;
		int max = 0;
		lowerBound = data[maxJobs - 1].submit + data[maxJobs - 1].run;
		//cout<<"DATA "<<data.begin()<<endl;
		sort(data.begin(), data.end(), sortfunc);//sort elements by ratio
												 //cout << "check";    //Debug
		for (int i = 0; i<WindowSize; i++)//throw to best elements by index
		{
			window[i] = counter;
			//cout<<window[i];
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
			grasp[counter1].index = window[selected];//insert into solution
													 //cout<<grasp[counter1].index<<" ";
			grasp[counter1].submit = data[grasp[counter1].index].submit;
			//int id=data[grasp[counter1].index].id;
			//cout<<grasp[counter1].submit<<" "<<id<<endl;
			//cout<<grasp[counter1]<<endl;
			counter1++;
			window[selected] = -1;//used element
		}
		check = 0;
		sort(grasp, grasp + maxJobs, sortfuncgrasp);//sort solution by submit time
		for (int i = 0; i<maxJobs; i++)
		{
			//cout<<grasp[i].index<<" "<<grasp[i].submit<<endl;
			if (time<grasp[i].submit) time = grasp[i].submit;//if submit time bigger than current time go to submit time
															 //cout<<time<<endl;
			for (int j = 0; j<maxNodes; j++)
			{
				if (procTab[j] <= time) check++;
			}
			//cout<<check<<endl;
			if (check >= data[grasp[i].index].proc)//if enough processors
			{
				//cout<<"good"<<endl;
				int finish = time + data[grasp[i].index].run;
				counter = 0;
				for (int j = 0; j<maxNodes; j++)//insert finish time for every occupied processor
				{
					if (procTab[j] <= time)
					{
						procTab[j] = finish;
						out[counter_out].processors.push_back(j);
						//cout<<j<<" ";
						counter++;
					}
					//cout<<counter<<endl;
					if (counter == data[grasp[i].index].proc) break;
				}
				out[counter_out].id = data[grasp[i].index].id;//add to output
				out[counter_out].start = time;
				out[counter_out].stop = finish;
				//cout<<out[counter_out].id<<" "<<out[counter_out].start<<" "<<out[counter_out].stop<<endl;
				output.push_back(out[counter_out]);
				counter_out++;
			}
			else
			{
				//time=1;
				//procTab[0]=0;
				while (check<data[grasp[i].index].proc)//while there is not enough free processors
				{
					check = 0;
					int j = 0;
					int min = procTab[0];
					while (procTab[j] <= time)//find lowest time when some processors are free
					{
						//cout<<"jestem"<<endl;
						min = procTab[j + 1];
						j++;
					}
					for (int j = 0; j<maxNodes; j++)
					{
						//cout<<procTab[j]<<endl;
						if (procTab[j]<min && time<procTab[j]) min = procTab[j];
					}
					//cout<<"min "<<min<<endl;
					time = min;
					for (int j = 0; j<maxNodes; j++)//check if enough free processors
					{
						if (procTab[j] <= time) check++;
					}
				}
				int finish = time + data[grasp[i].index].run;
				if (finish > max) max = finish;
				counter = 0;
				for (int j = 0; j<maxNodes; j++)//insert finish time for every occupied processor
				{
					if (procTab[j] <= time)
					{
						procTab[j] = finish;
						out[counter_out].processors.push_back(j);
						//cout<<j<<" ";
						counter++;
					}
					//cout<<counter<<endl;
					if (counter == data[grasp[i].index].proc) break;
				}
				out[counter_out].id = data[grasp[i].index].id;//add to output
				out[counter_out].start = time;
				out[counter_out].stop = finish;
				//cout<<out[counter_out].id<<" "<<out[counter_out].start<<" "<<out[counter_out].stop<<endl;
				output.push_back(out[counter_out]);
				counter_out++;
			}
			//cout<<endl;
			check = 0;
			if (obliczSekundy(clock()) >= 295)
			{
				printf("Zbyt duza instancja lub zabraklo czasu - przepraszamy.");
				break;
			}
		}
		for (int i = 0; i < maxNodes; i++)
		{
			sumTime += procTab[i];
			//cout<<sumTime<<" "<<procTab[i]<<endl;
		}
		return  max;
	}


	vector<Proc> const &getV() const {
		return output;
	}

	int getBound()
	{
		return lowerBound;
	}

	int getSumTime()
	{
		return sumTime;
	}

};

int main(int argc, char **argv)
{
	unsigned int sum;
	int num = 1;
	unsigned long long out1, out2 = 99992147483646;
	int bound;
	double  stop;
	Data tasklist;
	vector<Proc> best_output;
	char *p;
	long quant = strtol(argv[3], &p, 10);
	//cout<<quant<<endl;
	//overall_time = obliczSekundy(clock());
	tasklist.getData(argv[1], quant);
	//cout << "good";      //Debug
	//tasklist.showData();
	do {
		out1 = tasklist.GRASP();
		bound = tasklist.getBound();
		sum = tasklist.getSumTime();
		//ofstream plik1;
		//plik1.open("wynikispr-LANL-10000.txt", ios::out);
		cout << "out1 " << out1 << " bound " << bound << " sum " << sum << endl;
		if (out1 < out2)
		{
			out2 = out1;
			best_output = tasklist.getV();
			stop = obliczSekundy(clock());
			if (stop > 175 && num == 1) break;
		}
		//cout << out1 << endl;
		num++;
	} while (obliczSekundy(clock()) < 295 && num < 5);
	stop = obliczSekundy(clock());
	cout << stop << endl;
	ofstream plik;
	plik.open(argv[2], ios::out);
	for (int i = 0; i < best_output.size(); i++)
	{
		plik << best_output[i].id << " " << best_output[i].start << " " << best_output[i].stop << " ";
		for (int j = 0; j < best_output[i].processors.size(); j++) plik << best_output[i].processors[j] << " ";
		plik << endl;
	}
	plik.close();
	//plik1.close();
	//system("PAUSE");
	return 0;
}
