#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
using namespace std;


double probability(const int& i, const int& total)
{
    return (double) i / total;
}

double entropy(const double& prob1, const double& prob2)
{
    return (double) (-1) * prob1 * log2(prob1);
}


double ENTROPY(const vector<int>& target_v, const int& total)
{
    double res;
    int* p_ct = (int*)malloc(sizeof(int)*10);
    for (int i = 0; i < total; i++)
    {
        int ct =  count(begin(target_v), end(target_v), i);
        p_ct[i] = ct;
        double p1 = probability(p_ct[i], total);
        double p2 = probability(total - p_ct[i], total);
        double ent = entropy(p1, p2);
        res += ent;
        // cout << "::"<< ent << endl;
    }
    return (double) res;
}



double gain(const double& ent, const vector<double>& prob_v)
{
    return 0.;
}


void C45DT(map<string, vector<double>>& table, const vector<string>& head)
{
    for (string h_name : head)
    {   
        cout << h_name << " ";

        vector<double> curr_v = table[h_name];
        sort(begin(curr_v), end(curr_v));
        curr_v.erase(unique(curr_v.begin(), curr_v.end()), curr_v.end()); // 去重
        cout << curr_v.size() << endl;
        cout << endl;
    }
}





void split(vector<string>& st_result, const string& s, const string& text)
{
	int s_len = s.length();
	int start = 0;
	int index;
	while ((index = text.find(s, start)) != -1)
	{
		st_result.push_back(text.substr(start, index - start));
		start = index + s_len;
	}
	if (start < text.length())
	{
		st_result.push_back(text.substr(start, text.length() - start));
	}
}


int main(int argc, char const *argv[])
{
    fstream wine_f(argv[1]);
    string line;
    vector<string> head;
    map<string, vector<double>> table;
    getline(wine_f, line);
    split(head, ",", line);

    for (string s : head)
    {
        vector<double> values;
        table[s] = values;
    }
    int head_index = head.size();

    while (getline(wine_f, line))
    {
        vector<string> values;
        split(values, ",", line);
        for (int i = 0; i < head_index; i++)
        {
            table[head[i]].push_back(stod(values[i]));
        }
    }

    for (string s : head)
    {   cout << s << ":";
        for (double d : table[s])
        {
            cout << d << " ";
        }
        cout << "|" << table[s].size() << endl;
    }
    cout << endl;
    cout << "Hello" << endl;


    C45DT(table, head);


    // cout << probability(1, 14) << endl;

    // vector<int> target_v;
    // for (int i = 0; i < 10; i++)
    // {
    //     target_v.push_back(i);
    // }




    // int* p_arr = arr;

    // int arr[10];
    // int (*i_arr)[10] = &arr;
    // for (int i = 0; i < 10; i++)
    // {   
    //     cout << *(*i_arr+i) << endl;
    // }

    // cout << "*i_arr = " << *i_arr << endl;
    // cout << "sizeof(ptr) = "<< sizeof(i_arr) << 
    //     ", sizeof(*ptr) = " << sizeof(*i_arr) << endl;

    // cout << ENTROPY(target_v, 10) << endl;
    return 0;
}