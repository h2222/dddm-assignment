#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <regex>

using namespace std;

template <typename T>
void split(vector<T>& st_result, const string& s, const string& text)
{
	regex st(s);
	vector<string> result(sregex_token_iterator(begin(text), end(text), st, -1),
			sregex_token_iterator());
	for(auto i : result)
	{
		st_result.push_back(i);
	}
}


int main(int argc, char* argv[])
{
	/*
		that is how we split the string and save key value pairs into the map, which can be done in a second by python ! 
	*/
	vector<string> st_result;

	for (int i = 1; i < argc; i++)
	{
		string fname = argv[i];
		string att = "att";
		string que = "query";
		string acc = "acc";

		if (fname.find(att) != string::npos)
		{
			string as = "\\s+";
			cout << fname << endl;	
			map<string, string> attm;
			fstream myfile(argv[i]);
			string line;
			getline(myfile, line); // jump first line 
			while(getline(myfile, line))
			{
				split(st_result, as, line);
				attm.insert(pair<string, string>(st_result[0], st_result[1]));
				st_result.clear();
			}
			// print map contents
			map<string, string>::iterator iter;
			for (iter = begin(attm); iter != end(attm); iter++)
			{
				cout << iter->first << ":" << iter->second << endl;
			}
		} 
		else if (fname.find(que) != string::npos)
		{
			string qs = ":\\s+";
			cout << fname << endl;	
			map<string, string> quem;
			fstream myfile(argv[i]);
			string line;
			while(getline(myfile, line))
			{
				split(st_result, qs, line);
				quem.insert(pair<string, string>(st_result[0], st_result[1]));
				st_result.clear();
			}
			map<string, string>::iterator iter;
			for (iter = begin(quem); iter != end(quem); iter++)
			{
				cout << iter->first << ":" << iter->second << endl;
			}
		}
		else if (fname.find(acc) != string::npos)
		{
			string cs = "\\s+";
			vector<int> st_result_2;
			cout << fname << endl;	
			map<string, vector<int>> accm;
			fstream myfile(argv[i]);
			string line;
			getline(myfile, line);
			while(getline(myfile, line))
			{
				split(st_result, cs, line);
				string key = st_result[0];
				for (int i = 1; i < st_result.size(); i++)
				{
					int a  = stoi(st_result[i]);
					st_result_2.push_back(a);
				}
				accm.insert(pair<string, vector<int>>(key, st_result_2));
				st_result_2.clear();
				st_result.clear();
			}
			map<string, vector<int>>::iterator iter;
			for (iter = begin(accm); iter != end(accm); iter++)
			{
				cout << iter->first << ": ";
				vector<int> temp = iter->second;
				for (int i : temp)
				{
					cout << i << " ";
				}
				cout << endl;
			}
		}
	}
	return 0;
}
