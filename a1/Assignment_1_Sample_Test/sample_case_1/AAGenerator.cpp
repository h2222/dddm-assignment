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


void decide_query(string att, map<string, string> m, vector<string>& q)
{
	map<string, string>::iterator iter;
	string query;
	string::size_type att_idx;
	for(iter = begin(m); iter != end(m); iter++)
	{
		query = iter->second;
		att_idx = query.find(att);
		if (att_idx != string::npos)
		{
			q.push_back(iter->first);
		}
	} 
}



void generator(map<string, int>& attm, map<string, string>& quem, map<string, vector<int>>& accm)
{	
	map<string, int>::iterator iter1;
	vector<string> att;
	for (iter1 = begin(attm); iter1 != end(attm); iter1++)
	{	
		att.push_back(iter1->first);
		cout << iter1->first << ": " << iter1->second << endl;
	}

	vector<string> qi;
	vector<string> qj;
	for (int i = 0; i < att.size(); i++)
	{
		for (int j = 0; j < att.size(); j++)
		{
			cout << att[i] << " " << att[j] << endl;
			decide_query(att[i], quem, qi);
			decide_query(att[j], quem, qj);
			
			// Ai_query(1, 2, 4), Aj_query(2, 3) -> Ai_k=1 = (q1, S1) + (q1, S2) + (q1, S3) -> aff 
			for (string q : qi)
			{
				cout << q << " "; // query for Ai
			}
			cout <<" ";
			cout << endl;

			for (string q :qj)
			{
				cout << q << " "; // query for Aj
			}
			cout << endl;

			qi.clear();
			qj.clear();
		}
	}



	
	map<string, vector<int>>::iterator iter2;
	for (iter2 = begin(accm); iter2 != end(accm); iter2++)
	{
		cout << iter2->first << ": ";
		vector<int> temp = iter2->second;
		// for (int i : temp)
		// {

		// 	cout << i << " ";
		// }
		// cout << endl;
	}
	// cout << "good" << endl;
}



int main(int argc, char* argv[])
{
	/*
		that is how we split the string and save key value pairs into the map, which can be done in a second by python ! 
	*/
	vector<string> st_result;
	map<string, int> attm;
	map<string, string> quem;
	map<string, vector<int>> accm;

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
			fstream myfile(argv[i]);
			string line;
			getline(myfile, line); // jump first line 
			while(getline(myfile, line))
			{
				split(st_result, as, line);
				attm.insert(pair<string, int>(st_result[1], 0));
				st_result.clear();
			} 
		} 
		else if (fname.find(que) != string::npos)
		{
			string qs = ":\\s+";
			cout << fname << endl;	
			fstream myfile(argv[i]);
			string line;
			while(getline(myfile, line))
			{
				split(st_result, qs, line);
				quem.insert(pair<string, string>(st_result[0], st_result[1]));
				st_result.clear();
			}
		}
		else if (fname.find(acc) != string::npos)
		{
			string cs = "\\s+";
			vector<int> st_result_2;
			cout << fname << endl;	
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
		}
	}
	generator(attm, quem, accm);
	return 0;
}
