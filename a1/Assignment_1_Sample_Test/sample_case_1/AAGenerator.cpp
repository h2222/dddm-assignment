#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <cmath>

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


bool isnum(char n)
{
	return (n >= '0'&&n <= '9');
}


int* get_num_from_string(string & ch)
{
	int k = ch.size();
	int* num = new int[k];
	int result;
	int n = 0;
	int i = 0;
	while (n<k) 
	{
		result = 0;
		if (isnum(ch[n]))
		{
			result = ch[n] - '0';
			while (n < k && isnum(ch[++n]))
				result = (ch[n] - '0') + 10 * result;
				num[i++] = result;
		}
		++n;
	}
	return num;
}


void get_q_from_att(string att, map<string, string> m, vector<string>& q)
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


map<string, int> get_A_from_q(vector<string>& q_list, map<string, vector<int>>& accm)
{
	map<string, int> A_q;
	for (string q : q_list)
	{
		int A = 0;
		for (int i : accm[q])
		{
			A += i;
		}
		A_q.insert(pair<string, int>(q, A));
	}
	return A_q;
}


int get_AA_from_A(map<string, int>& A_i_q, map<string, int>& A_j_q, vector<string>& que_names)
{
	int sum_A_i = 0;
	int sum_A_j = 0;
	int sum_A_i_j = 0;
	double sqrt_mul_Ai_Aj = 0.1;
	int result = 0;
	map<string, int>::iterator iter2;
	for (iter2 = begin(A_i_q); iter2 != end(A_i_q); iter2++)
	{
		sum_A_i += iter2->second;
	}
	for (iter2 = begin(A_j_q); iter2 != end(A_j_q); iter2++)
	{
		sum_A_j += iter2->second;
	}
	for (string n : que_names)
	{
		if (A_i_q.count(n) != 0 && A_j_q.count(n) != 0)
		{
			sum_A_i_j += (A_i_q[n] * A_j_q[n]);
		}
	}
	sqrt_mul_Ai_Aj = sqrt(sum_A_i * sum_A_j);
	result =  ceil(sum_A_i_j / sqrt_mul_Ai_Aj);
	sum_A_i_j = 0;
	sum_A_i = 0;
	sum_A_j = 0;
	return result;
}


void generator(map<string, string>& attm, map<string, string>& quem, map<string, vector<int>>& accm)
{	
	map<string, string>::iterator iter1;
	vector<string> att_labels;
	vector<string> que_names;
	for (iter1 = begin(attm); iter1 != end(attm); iter1++)
	{	
		att_labels.push_back(iter1->first);
	}
	for (iter1 = begin(quem); iter1 != end(quem); iter1++)
	{
		que_names.push_back(iter1->first);
	}
	vector<string> qi;
	vector<string> qj;
	int* cuti;
	int* cutj;
	int** aa_matrix = new int* [att_labels.size()];
	// int aa_matrix[att_labels.size()][att_labels.size()];
	for (int i = 0; i < att_labels.size(); i++)
	{
		aa_matrix[i] = new int [att_labels.size()];
		for (int j = 0; j < att_labels.size(); j++)
		{
			// A1, A1 -> A1, A2 -> A1, A3
			cuti = get_num_from_string(att_labels[i]);
			cutj = get_num_from_string(att_labels[j]);
			get_q_from_att(attm[att_labels[i]], quem, qi);
			get_q_from_att(attm[att_labels[j]], quem, qj);
			// Ai_query(1, 2, 4), Aj_query(2, 3) -> Ai_k=1 = (q1, S1) + (q1, S2) + (q1, S3) -> aff 
			map<string, int> A_i_q = get_A_from_q(qi, accm);
			map<string, int> A_j_q = get_A_from_q(qj, accm);
			int aa_result = get_AA_from_A(A_i_q, A_j_q, que_names);
			aa_matrix[cuti[0]-1][cutj[0]-1] = aa_result;
			qi.clear();
			qj.clear();
		}
	}
	for (int i = 0; i < att_labels.size(); i++)
	{
		for (int j = 0; j < att_labels.size(); j++)
		{
			cout << aa_matrix[i][j] << " ";
		}
		cout << endl;
	}
}


int main(int argc, char* argv[])
{
	/*
		that is how we split the string and save key value pairs into the map, which can be done in a second by python ! 
	*/
	vector<string> st_result;
	map<string, string> attm;
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
			fstream myfile(argv[i]);
			string line;
			getline(myfile, line); // jump first line 
			while(getline(myfile, line))
			{
				split(st_result, as, line);
				attm.insert(pair<string, string>(st_result[0], st_result[1]));
				st_result.clear();
			} 
		} 
		else if (fname.find(que) != string::npos)
		{
			string qs = ":\\s+";
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