#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <regex>
#include <cmath>

using namespace std;





template <typename T>
void split(vector<T>& st_result, string& s, string& text)
{
	regex st(s);
	vector<string> result(sregex_token_iterator(begin(text), end(text), st, -1),
			sregex_token_iterator());
	for(string i : result)
	{
		st_result.push_back(i);
	}
}


int match(string target, string text)
{
	text = " " + text + " ";
	int len_s = text.size();
	int len_t = target.size();
	int count = 0;
	for (int i = 0; i < len_s - len_t; i++)
	{
		int j = 0;
		while (j < len_t && target[j] == text[i+j])
		{
			j++;
		}
		if (j == len_t)
		{
			char pre = text[i - 1];
			char back = text[i + len_t];
			string AZaz = "ABCDEFGHIJKLMNOPQRSTUVWSYZabcdefghijklmnopqrstuvwxyz";
			int posp = AZaz.find(pre);
			int backp = AZaz.find(back);
			if (posp == string::npos && backp == string::npos)
			{
				count++;
			}
		}
	}
	return count;
}


bool isnum(char n)
{
	return (n >= '0' && n <= '9');
}


int* get_num_from_string(string ch)
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


void get_q_from_att(string att, map<string, string> m, map<string, int>& q)
{
	map<string, string>::iterator iter;
	string query;
	for(iter = begin(m); iter != end(m); iter++)
	{
		query = iter->second;
		int use = match(att, query);
		if (use != 0) 
		{
			q.insert(pair<string, int>(iter->first, 1));
		}
		
	} 
}


map<string, int> get_A_from_q(map<string, int>& q_map, map<string, vector<int>>& accm)
{
	map<string, int> A_q; // <q, total A value for a att i>	
	map<string, int>::iterator iter;
	bool no_query = true;
	for(iter = begin(q_map); iter != end(q_map); iter++)
	{
		string q = iter->first;
		int use = iter->second;
		int A = 0;
		for (int i : accm[q])
		{
			A += use * i;  // + 1 x sum(access_map[q])
		}
		// cout << "query: " << q << ", A: " << A << endl;
		A_q.insert(pair<string, int>(q, A));
		no_query = false;
	}
	if (no_query == true)
	{
		// cout << "query: presudo" << ", A: 0" << endl;
		A_q.insert(pair<string, int>("presudo", 0));
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

	if (sum_A_i_j == 0)
	{
		result = 0;
	}
	else
	{
		result =  ceil(sum_A_i_j / sqrt_mul_Ai_Aj);
	}
	sum_A_i_j = 0;
	sum_A_i = 0;
	sum_A_j = 0;
	return result;
}


bool comp(string a, string b)
{
	int* an = get_num_from_string(a);
	int* bn = get_num_from_string(b);
	return an[0] < bn[0];
}


void vsort(vector<string>& v)
{
	int len = v.size();
	int a_rr[len];
	for(int i = 1; i < len; i++)
	{
		for (int j = 0; j < len - i; j++)
		{
			int* an = get_num_from_string(v[j]);
			int* bn = get_num_from_string(v[j+1]);
			if (an[0] > bn[0])
			{
				string temp = v[j];
				v[j] = v[j+1];
				v[j+1] = temp;
			}
		}
	}
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
	vsort(att_labels);
	for (iter1 = begin(quem); iter1 != end(quem); iter1++)
	{
		que_names.push_back(iter1->first);
	}
	vsort(que_names);
	map<string, int> qi; // <qk， qk_use>
	map<string, int> qj;
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
			// cout << "cut: " << cuti[0] << " cutj: " << cutj[0] << endl;
			get_q_from_att(attm[att_labels[i]], quem, qi);
			get_q_from_att(attm[att_labels[j]], quem, qj);
			// Ai_query(1, 2, 4), Aj_query(2, 3) -> Ai_k=1 = (q1, S1) + (q1, S2) + (q1, S3) -> aff 
			// cout << " att_i:" << att_labels[i] << " att: " << attm[att_labels[i]] << endl;
			map<string, int> A_i_q = get_A_from_q(qi, accm);
			// cout << " att_j:" << att_labels[j] << "att: " << attm[att_labels[j]] << endl;
			map<string, int> A_j_q = get_A_from_q(qj, accm);
			// cout << "-----" << endl;
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
				// cout << st_result[0] << " : " << st_result[1] << endl;
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
				// cout << st_result[0] << " : " << st_result[1] << endl;
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
				// cout << st_result[0] << " : " << st_result_2.size() << endl;
				accm.insert(pair<string, vector<int>>(key, st_result_2));
				st_result_2.clear();
				st_result.clear();
			}
		}
	}
	generator(attm, quem, accm);
	return 0;
}