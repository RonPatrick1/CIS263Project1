/********************************************************************
*	
*	Project 1
*	CIS 263
*	Summer Session
*	Ron Patrick
* 
*	This program takes a specific set of 'essay' data and searches
*		for signs of plagarism.  It can work with large sets of
*		data or small sets.
*	The data has to be in specific directories in order for
*		the program to find it, however.
*	This source code will compile correctly on linux with g++, or
*		with Windows with MS Visual Studio Express
*
*	g++ requires this command-line however for threading --
*		>g++ Project1.cpp -lpthread -o Project1 -std=c++11
*	
*	See the README.txt for more details.
*
*********************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd /*_getcwd*/
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#define GetCurrentDir getcwd
#endif

#include <string>
#include <math.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <queue>
#include <thread>
#include "RonsHuffmanCode.h"
/*#include "BST.h"*/
#include "Node.h"
#include <ctime>
#include <chrono>
/*#include <conio.h>*/
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

/*Globals*/
vector<string> FileNames;
unordered_map<string, int> words;
size_t dSubStringLength;
int Threshold;
int nWordSeq;
bool doPrintOut;
bool doAllRes;
bool doBeforeNoise;
Node *nlistc;

/*Function Templates*/
void Pause();
void GetFilesInDirectory(vector<string> &out, const string &directory);

/*struct I used to store the results*/
struct results {
	string DocOne;
	string DocTwo;
	int hits;
};
vector<results> Results;

/*struct I used to load all the files intoo*/
struct fileStruct {
	string filename;
	/*vector<int> data;*/
	vector<string> data;
};
vector<fileStruct> Files;

/*struct I used to pass data to my threads(linux)*/
struct threadData {
	int i;
	int j;
};

/*This is the thread used by windows to compare the data*/
#ifdef _WINDOWS_
void CompareNumsWindowsThread(int i, int j) {
	if (j >= FileNames.size()) {
		return;
	}
	/*vector<int> cfile = Files.at(i).data;
	vector<int> tfile = Files.at(j).data;
	string cname = FileNames.at(i);
	string tname = FileNames.at(j);
	queue<int> compQ;
	queue<int> tempQ;
	results r;
	if (cfile.size() >= nWordSeq && tfile.size() >= nWordSeq) {
		cname = cname.substr(dSubStringLength + 1, cname.length() - 1);
		tname = tname.substr(dSubStringLength + 1, tname.length() - 1);
		r.DocOne = cname;
		r.DocTwo = tname;
		r.hits = 0;
		for (vector<int>::iterator TempIt = tfile.begin(); TempIt != 
				tfile.end(); ++TempIt) {
			tempQ.push(*TempIt);
			if (tempQ.size() == nWordSeq) {
				for (vector<int>::iterator CompIt = cfile.begin(); 
						CompIt != cfile.end(); ++CompIt) {
					compQ.push(*CompIt);
					if (compQ.size() == nWordSeq) {
						if (compQ == tempQ) {
							r.hits++;
						}
						compQ.pop();
					}
				}
				tempQ.pop();
			}
		}
		if (r.hits >= Threshold || doAllRes) {
			Results.push_back(r);
		}
	}*/
}
/*This is the thread used by Linux to compare data files*/
#else
void *CompareNumsLinuxThread(void *threadarg) {

	struct threadData *my_data;
	my_data = (struct threadData *) threadarg;
	/*int i = my_data->i;
	int j = my_data->j;

	if (i >= FileNames.size() || j >= FileNames.size()) {
		return 0;
	}
	vector<int> cfile = Files.at(i).data;
	vector<int> tfile = Files.at(j).data;
	string cname = FileNames.at(i);
	string tname = FileNames.at(j);
	queue<int> compQ;
	queue<int> tempQ;
	results r;
	if (cfile.size() >= nWordSeq && tfile.size() >= nWordSeq) {
		cname = cname.substr(dSubStringLength + 1, cname.length() - 1);
		tname = tname.substr(dSubStringLength + 1, tname.length() - 1);
		r.DocOne = cname;
		r.DocTwo = tname;
		r.hits = 0;
		for (vector<int>::iterator TempIt = tfile.begin(); TempIt != 
				tfile.end(); ++TempIt) {
			tempQ.push(*TempIt);
			if (tempQ.size() == nWordSeq) {
				for (vector<int>::iterator CompIt = cfile.begin();
						CompIt != cfile.end(); ++CompIt) {
					compQ.push(*CompIt);
					if (compQ.size() == nWordSeq) {
						if (compQ == tempQ) {
							r.hits++;
						}
						compQ.pop();
					}
				}
				tempQ.pop();
			}
		}
		if (r.hits >= Threshold || doAllRes) {
			Results.push_back(r);
		}
	}*/
}
#endif

/*Main entry point ******************************************/
int main(int argc, char *argv[]) {


	/*string tests = "\ttest\tthis\t\tthing\n";
	vector<string> testsp;
	split(testsp, tests, is_any_of("\t"));
	for (string::iterator i = tests.begin(); i != tests.end(); ++i) {
			if ((*i)=='\t') {
				cout << "tab" << endl;
			}
	}
	Pause();
	return 0;*/

	/*cout << "test: ";
	char gc = _getch();
	return 0;*/

	/*Throughout the program, I needed to write alternate code
		for Linux and Windows, so I just did it this way.   */
#ifdef _WINDOWS_
	string slash = "\\";
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	int numCPU = sysinfo.dwNumberOfProcessors;
#else
	string slash = "/";
	int numCPU = sysconf(_SC_NPROCESSORS_ONLN);
#endif

	/* A million questions for the user to start the program*/
	bool doThreads = false;
	/*if (numCPU > 2) {*/
		cout << "I detected " << numCPU << " CPUs." << endl;
	/*	numCPU--;
		cout << "May I use " << numCPU <<
		" threads to process the files(y/n)?" << endl;
		string input;
		cin >> input;
		if (input == "y" || input == "yes" || input == "Yes") {
		doThreads = true;
		}
		}*/
	doThreads = true;
	if (!doThreads) {
		numCPU = 1;
	}

	cout << endl << "Should I use the big(b), the medium(m), or the ";
	cout << "small(s) file set?" << endl;
	string input = "";
	while (input != "b" && input != "m" && input != "s") {
		cout << "(b/m/s): ";
		cin >> input;
	}

	string docFolder;
	if (input == "b") {
		docFolder = slash + "big_doc_set" + slash;
	}
	if (input == "m") {
		docFolder = slash + "med_doc_set" + slash;
	}
	if (input == "s") {
		docFolder = slash + "sm_doc_set" + slash;
	}

	cout << endl << "How long of a word sequence should I look for?"
		<< endl;
	int inSeq = -1;
	while (inSeq < 2) {
		cout << "(default is 6): ";
		cin >> inSeq;
	}
	nWordSeq = inSeq;

	cout << endl << "How many " << inSeq << "-word sequences ";
	cout << "should be the threshold ? " << endl;
	int inThresh = -1;
	while (inThresh < 5 || inThresh > 1500) {
		cout << "(between 5 and 1500 - default is 100): ";
		cin >> inThresh;
	}
	Threshold = inThresh;
	cout << endl;

	cout << "Would you like me to print out all " << inSeq <<
		"-word sequences in all files for testing?";
	cout << endl;
	input = "";
	doPrintOut = false;
	doBeforeNoise = false;
	/*while (input != "n" && input != "y") {
		cout << "(y/n): ";
		cin >> input;
		}
		if (input == "y") {
		doPrintOut = true;
		cout << endl << "Would you like file-contents before or";
		cout << " after noise reduction?\n";
		input = "";
		while (input != "b" && input != "a") {
		cout << "(b/a): ";
		cin >> input;
		}
		if (input == "b") {
		doBeforeNoise = true;
		}
		}*/

	cout << endl << "Would you like to print out all results?\n";
	cout << "(No means only results whose threshold is above the ";
	cout << "limit will be shown)\n";
	doAllRes = false;
	input = "";
	/*while (input != "n" && input != "y") {
		cout << "(y/n): ";
		cin >> input;
		}
		cout << endl;
		if (input == "y") {
		doAllRes = true;
		}*/

	char CurrentPath[FILENAME_MAX];
	if (!GetCurrentDir(CurrentPath, sizeof(CurrentPath)))	{
		return errno;
	}

	docFolder = CurrentPath + docFolder;
	dSubStringLength = docFolder.length();

	GetFilesInDirectory(FileNames, docFolder);

	/*Calculate how many actual file comparisions we need to do*/
	int numOfComparisions = (int)FileNames.size() - 1;
	int count = 0;
	while (numOfComparisions != 0) {
		count += numOfComparisions;
		numOfComparisions--;
	}
	numOfComparisions = count;
	count = 0;

	/*Here I load in the files and convert all the text to lower case.
		I create a dictionary of all the words used in all the essays in
		one particular data set without duplicates.
		I then use the unique 'bucket' number given to each word in the
		dictionary of an unorder_map to convert each essay into a
		sequence of 'int's, therefore making it so much easier to load every
		essay into memory at one time.  Last I checked I loaded all essays in
		the 'big' data set into memory and it only took like 12 megabytes.

		I kept track of how many times each word showed up in all of
		the essays,	because I was intending on turning each eassy
		into a Huffman Code where each word had it's own unique
		binary code, but I didn't have time.  That would've saved even
		more memory.  So while I'm creating the	dictionary of words,
		the 'second' in the pair in the unordered_map, was the
		frequency in which that word was used across all essays in
		a data set*/

	/*RonsHuffmanCode<string> huffCode;*/
	clock_t myTick = clock();
	chrono::time_point<std::chrono::system_clock> start, end;
	start = chrono::system_clock::now();


	string line;
	vector <string> tokens;
	vector <string> testPrint;
	string fname;
	int wordCount = 0;
	int totalWords = 0;
	vector<double> freqPercent;
	for (int i = 0; i < FileNames.size(); i++) {

		fileStruct f;
		f.filename = FileNames.at(i);
		fname = f.filename;
		cout << "Building dictionary " << (int)(100.0*((float)(i + 1) /
			(float)FileNames.size()));
		cout << "%";
		fname = fname.substr(dSubStringLength + 1, fname.length() - 1);
		cout << " with file: " << fname << "                        \r";

		ifstream myfile(FileNames.at(i));
		if (myfile.is_open()) {
			string tempstr;
			while (getline(myfile, line)) {
				if (doBeforeNoise && doPrintOut) {
					cout << line;
				}
				split(tokens, line, is_any_of(", .!-?;:'(){}~"));

				/*Here I'm converted all words to lower case, but not
				it the original file, of course, and removing
				puncuation noise from the dictionary at the same time.*/
				for (vector<string>::iterator it = tokens.begin(); it
					!= tokens.end(); ++it){
					if (*it != "") {
						tempstr = *it;
						string tempLowerCaseStr;
						char *chr = new char[tempstr.length() + 1];
#ifdef _WINDOWS_
						strcpy_s(chr, tempstr.length() + 1,
							tempstr.c_str());
#else
						strcpy(chr, tempstr.c_str());
#endif
						for (int j = 0; j < tempstr.length(); j++) {
							int num = (int)chr[j];
							if (num > 64 && num < 91) {
								chr[j] += 32;
							}
							num = (int)chr[j];
							if ((num > 96 && num < 129) ||
								(num>47 && num < 58)) {
								tempLowerCaseStr += chr[j];
							}
						}
						auto search = words.find(tempLowerCaseStr);
						if (search == words.end()) {
							words.insert(
								pair<string, int>(tempLowerCaseStr, 1));
							wordCount++; 
						}
						else {
							search->second++;
						}
						totalWords++;
						/*This is only used if the user needs proof the
						files are being read in properly.*/
						if (doPrintOut) {
							testPrint.push_back(tempLowerCaseStr);
						}
						/*f.data.push_back((int)words.bucket(tempLowerCaseStr));*/
						f.data.push_back(tempLowerCaseStr);
						delete[] chr;
					}
				}
			}
			myfile.close();
			if (doPrintOut && doBeforeNoise) {
				cout << "\n\n\n";
				Pause();
			}
			Files.push_back(f);

			/*This giant 'if' statement just prints out the file
			contents and every single n-word sequence in the file.*/
			if (doPrintOut) {
				cout << "\n\n\n";
				if (!doBeforeNoise) {
					cout << fname << " full text output: " << endl;
					for (vector<string>::iterator it = testPrint.begin();
						it != testPrint.end(); ++it) {
						cout << *it << " ";
					}
					cout << "\n\n";
					Pause();
				}
				cout << fname << ": All " << nWordSeq <<
					"-word sequences: \n";
				queue<string> testpq;
				int pauseScrollingAfterABit = 0;
				for (vector<string>::iterator it = testPrint.begin();
					it != testPrint.end(); ++it) {
					testpq.push(*it);
					if (testpq.size() == nWordSeq) {
						pauseScrollingAfterABit++;
						string c;
						cout << " | ";
						for (int l = 0; l < testpq.size(); l++) {
							c = testpq.front();
							cout << c << " ";
							testpq.pop();
							testpq.push(c);
						}
						testpq.pop();
					}
					/*if (pauseScrollingAfterABit > 100) {
						cout << "\n\n";
						Pause();
						pauseScrollingAfterABit = 0;
						}*/
				}
				cout << "\n\n";
				Pause();
			}
		}
		else {
			cout << "Unable to open file";
			return 0;
		}
	}

	unordered_map<string, int> seqs;
	unordered_map<int, string> links;
	cout << "\n\n";
	string ts;
	string ffname;
	int seqCount = 0;
	for (vector<fileStruct>::iterator it = Files.begin(); it != Files.end(); ++it) {
		ffname = it->filename;
		ffname = ffname.substr(dSubStringLength + 1, ffname.length() - 1);
		queue<string> compQ;
		cout << "File: " << ffname << "               \r";
		for (int i = 0; i < it->data.size(); i++) {
			compQ.push(it->data.at(i));
			if (compQ.size() == nWordSeq) {
				queue<string> tt;
				tt = compQ;
				ts = tt.front();
				tt.pop();
				for (int j = 1; j < nWordSeq; j++) {
					ts = ts + " " + tt.front();
					tt.pop();
				}
				auto search = seqs.find(ts);
				if (search == seqs.end()) {
					seqs.insert(pair<string, int>(ts, seqCount));
					links.insert(pair<int, string>(seqCount, ffname));
					seqCount++;
				}
				else {
					auto search2 = links.find(search->second);
					vector<string> fcheck;
					split(fcheck, search2->second, is_any_of(" "));
					bool dontdoit = false;
					for (int k = 0; k < fcheck.size(); k++) {
						if (fcheck.at(k) == ffname) {
							dontdoit = true;
						}
					}
					if (!dontdoit) {
						search2->second += (" " + ffname);
					}
				}
				compQ.pop();
			}
		}
	}
	cout << endl << endl;

	unordered_map<string, int> output;
	for (unordered_map<int, string>::iterator it = links.begin(); it != links.end(); ++it) {
		vector<string> hits;
		split(hits, it->second, is_any_of(" "));
		if (hits.size()>1) {
			for (int i = 0; i < hits.size(); i++) {
				for (int j = i + 1; j < hits.size(); j++) {
					string comp = hits.at(i) + " " + hits.at(j);
					/*cout << comp << endl;*/
					string comp2 = hits.at(j) + " " + hits.at(i);
					auto search = output.find(comp);
					if (search != output.end()) {
						search->second++;
					}
					else {
						search = output.find(comp2);
						if (search != output.end()) {
							search->second++;
						}
						else{
							output.insert(pair<string, int>(comp, 1));
						}
					}
				}
			}
		}
	}
	cout << "\n\n";
	for (unordered_map<string, int>::iterator it = output.begin(); it != output.end(); ++it) {
		if (it->second >= Threshold) {
			vector<string> fsplit;
			split(fsplit, it->first, is_any_of(" "));
			cout << "Hits: " << it->second << "\t" << " Files: " << fsplit.at(0) << "\t";
			if (fsplit.at(0).length() < 15) {
				cout << "\t";
			}
			cout << fsplit.at(1) << endl;
		}
	}
	cout << "Time: " << (clock() - myTick) / (double)(CLOCKS_PER_SEC /1000) << " ms" << endl;
	end = std::chrono::system_clock::now();
	int elapsed_seconds = chrono::duration_cast<chrono::milliseconds> (end - start).count();
	time_t end_time = chrono::system_clock::to_time_t(end);
	cout << "Finished computation at " << ctime(&end_time) << "elapsed time: " << elapsed_seconds << " ms\n";

	Pause();
	return 0;
	


	vector<Node> nlist;
	for (unordered_map<string, int>::iterator it = words.begin(); it != words.end(); ++it) {
		Node tempNode((double)it->second / (double)totalWords, it->first);
		nlist.push_back(tempNode);
	}
	vector<Node> newNodeList;
	unordered_map<int, bool> erased;
	cout << endl << endl;
	int test = 0;
	int ttempr = 0;
	int nlisttrack = nlist.size();
	
	nlistc = (Node *) calloc((nlisttrack*2) + 1, sizeof(Node));
	int nlistcIndex = 0;
	for (nlistcIndex = 0; nlistcIndex < nlist.size(); nlistcIndex++) {
		nlistc[nlistcIndex] = nlist.at(nlistcIndex);
		nlistc[nlistcIndex].myIndex = nlistcIndex;
	}
	while (nlisttrack > 1) {
		double min = 1.1;
		Node *rnode;
		Node *lnode;

		int tempr;
		for (int i = 0; i < nlist.size(); i++) {
			if (nlist.at(i).freq < min && erased.find(i)==erased.end()) {
				min = nlist.at(i).freq;
				rnode = &nlistc[i];
				tempr = i;
			}
		}

		erased.insert(pair<int,bool>(tempr,false));

		nlisttrack--;
		nlistc[tempr].ZeroOne = 1;
		Node n;
		n.rchild = &nlistc[tempr];
		min = 1.1;
		int templ;
		for (int i = 0; i < nlist.size(); i++) {
			if (nlist.at(i).freq < min && erased.find(i) == erased.end()) {
				min = nlist.at(i).freq;
				lnode = &nlistc[i];
				templ = i;
			}
		}
		erased.insert(pair<int, bool>(templ, false));
		nlisttrack--;
		nlistc[templ].ZeroOne = 0;
		n.lchild = &nlistc[templ];
		n.freq = n.lchild->freq + n.rchild->freq;
		n.word = n.lchild->word + " " + n.rchild->word;

		nlisttrack++;
		nlist.push_back(n);
		nlistc[nlistcIndex] = n;
		nlistc[tempr].parent = &nlistc[nlistcIndex];
		nlistc[templ].parent = &nlistc[nlistcIndex];
		nlistc[nlistcIndex].myIndex = nlistcIndex;
		nlistcIndex++;
	}

	nlistcIndex--;
	unordered_map<string, string> code;
	vector<string> splits;
	string currentCode = "";
	unordered_map<int, bool> visited;
	while (code.size() != wordCount) {
		visited.insert(pair<int,bool>(nlistcIndex,false));
		split(splits, nlistc[nlistcIndex].word, is_any_of(" "));
		if (splits.size() == 1) {
			auto search = code.find(splits.at(0));
			if (search == code.end()) {
				code.insert(pair<string, string>(nlistc[nlistcIndex].word, currentCode));
			}
		}
		if (nlistc[nlistcIndex].lchild != NULL && visited.find(nlistc[nlistcIndex].lchild->myIndex) == visited.end()) {
			currentCode = currentCode + to_string(nlistc[nlistcIndex].lchild->ZeroOne);
			nlistcIndex = nlistc[nlistcIndex].lchild->myIndex;
		}
		else {
			if (nlistc[nlistcIndex].rchild != NULL && visited.find(nlistc[nlistcIndex].rchild->myIndex) == visited.end()) {
				currentCode = currentCode + to_string(nlistc[nlistcIndex].rchild->ZeroOne);
				nlistcIndex = nlistc[nlistcIndex].rchild->myIndex;
			}
			else {
				if (nlistc[nlistcIndex].parent != NULL) {
					currentCode = currentCode.substr(0, currentCode.length() - 1);
					nlistcIndex = nlistc[nlistcIndex].parent->myIndex;
				}
			}
		}
	}
	cout << "\nDone.\n\n";
	int maxLength = 0;
	for (unordered_map<string, string>::iterator it = code.begin(); it != code.end(); ++it) {
		if (it->second.length() > maxLength) {
			maxLength = it->second.length();
		}
	}
	cout << maxLength << endl;
	Pause();
	return 0;

	cout << "Done." << endl;
	return 0;

	/*Separate code for Windows Threads and Linux threads
	I didn't have time to mess with thread-affinity to bind
	my threads to a specific cpu core, but I've done that 
	before.  I realize my threads may or may not be actually
	running on separate cores.  This code still runs at least 
	twice as fast this way, however.*/
#ifdef _WINDOWS_
	vector<thread> t;
#else
	vector<pthread_t> t;
	vector<threadData> td;
	threadData emptyThreadData;
	pthread_t emptyThread;
	pthread_attr_t attr;
	void *status;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
#endif
	for (int i = 0; i < FileNames.size(); i++) {
		int j = i + 1;
		while (j < FileNames.size()) {
			int numOfThreadsActual = 0;
			for (int l = 0; l < numCPU && j < FileNames.size(); l++) {
				cout << "Comparing files ";
				cout << (int)(100.0*((float)count / 
					(float)numOfComparisions));
				cout << "% " << count << " of " << numOfComparisions;
				cout << "          \r";
				count++;
				numOfThreadsActual++;
#ifdef _WINDOWS_
				t.push_back(thread(CompareNumsWindowsThread, i, j));
#else
				emptyThreadData.i=i;
				emptyThreadData.j=j;
				td.push_back(emptyThreadData);
				t.push_back(emptyThread);
				int rc = pthread_create(&t.back(), NULL, 
					CompareNumsLinuxThread, (void *)&td.back());
				if (rc){
					cout << "Thread creation error," << rc << endl;
					return 0;
				}
#endif
				j++;
			}
			for (int l = 0; l < numOfThreadsActual; l++) {
#ifdef _WINDOWS_
				t.at(l).join();
#else
				pthread_join(t.at(l), &status);
#endif
			}
			if (!t.empty()) {
				t.clear();
			}
		}
	}
	cout << "\nDone.\n\n";

	/*Now we're done.  Just sorting out the results here.
	Largest hits to smallest hits.*/
	int minSort;
	int sIndex;
	vector<results> SortedList;
	while (Results.size()) {
		minSort = -1;
		for (int i = 0; i < Results.size(); i++) {
			if (Results.at(i).hits > minSort) {
				minSort = Results.at(i).hits;
				sIndex = i;
			}
		}
		SortedList.push_back(Results.at(sIndex));
		Results.erase(Results.begin()+sIndex);
	}
	cout << "Results by rank.  ";
	cout << "The first in the list are the most suspicious ones.\n\n";
	for (int i = 0; i < SortedList.size(); i++) {
		cout << SortedList.at(i).hits << "\t\t";
		cout << SortedList.at(i).DocOne << ", \t";
		if (SortedList.at(i).DocOne.length() < 15) {
			cout << "\t";
		}
		cout << SortedList.at(i).DocTwo;
		cout << "\n";
	}
	cout << endl;
	Pause();
	return 0;
}

void Pause() {

#ifdef _WINDOWS_
	system("pause");
#else
	/*I can't find a good way to get a system("pause") in
	linux, but most of the time it's not necessary.  This
	works for now.*/
	cout << "Press enter to continue . . . ";
	cin.ignore(cin.rdbuf()->in_avail()+1);
	/*getchar();*/
#endif

}

/*My Windows/Linux function that returns a list of all 
the files in a directory*/
void GetFilesInDirectory(vector<string> &out, const string &directory) {

#ifdef _WINDOWS_
	HANDLE dir;
	WIN32_FIND_DATAA file_data;

	if ((dir = FindFirstFileA((directory + "/*").c_str(), &file_data)) 
			== INVALID_HANDLE_VALUE)
		return;

	do {
		const string file_name = file_data.cFileName;
		const string full_file_name = directory + "/" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & 
			FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.')
			continue;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	} while (FindNextFileA(dir, &file_data));

	FindClose(dir);
#else
	DIR *dir;
	class dirent *ent;
	class stat st;

	dir = opendir(directory.c_str());
	while ((ent = readdir(dir)) != NULL) {
		const string file_name = ent->d_name;
		const string full_file_name = directory + "/" + file_name;

		if (file_name[0] == '.')
			continue;

		if (stat(full_file_name.c_str(), &st) == -1)
			continue;

		const bool is_directory = (st.st_mode & S_IFDIR) != 0;

		if (is_directory)
			continue;

		out.push_back(full_file_name);
	}
	closedir(dir);
#endif
}

