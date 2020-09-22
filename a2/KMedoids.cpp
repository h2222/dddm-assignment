#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

const string arrive_time_k = "arrive_time";
const string package_len_k = "package_len";
const string ave_arr_time_k = "ave_arrive_time";
const string ave_pk_len_k = "ave_package_len";

void split_by_space(vector<string>& st_result, string& text)
{
    int str_len = text.length();
    int start = 0;
    string match = "1234567890.";
    for (int i = 0; i < str_len; i++)
    {
        int idx = match.find(text[i]);
        if (idx == -1)
        {
            st_result.push_back(text.substr(start, i - start));
            start = i + 1;
        }
        if (i == str_len - 1)
        {
            st_result.push_back(text.substr(start, str_len - start));
        }
    }
}


map<string, map<string, int>> compute(map<string, map<string, vector<int>>>& addr_m)
{
    map<string, map<string, int>> comp_res;
    map<string, map<string, vector<int>>>::iterator iter;
    for (iter = begin(addr_m); iter != end(addr_m); iter++)
    {
        string addr = iter->first;
        // cout << "address: " << addr << endl;
        map<string, vector<int>> cm = iter->second;
        vector<int> arr_t_v = cm.find(arrive_time_k)->second;
        vector<int> pk_len_v = cm.find(package_len_k)->second;
        int len_arr_t_v = arr_t_v.size();
        int len_pk_t_v = pk_len_v.size();
        float ave_arr_t = 0.;
        float ave_pk_len = 0.;
        // calculating the average transferring time of first flow
        for (int i = len_arr_t_v - 1; i >= 1; i--)
        {
            ave_arr_t += (arr_t_v[i] - arr_t_v[i-1]);
        }
        ave_arr_t /= (len_arr_t_v - 1 + 1e-15);
        // cout << ave_arr_t << endl;
        // calculating the average length of first flow
        for (int pk_len : pk_len_v)
        {
            ave_pk_len += pk_len;
        }
        ave_pk_len /= len_pk_t_v;
        // cout << ave_pk_len << endl;

        comp_res[addr][ave_arr_time_k] = ave_arr_t;
        comp_res[addr][ave_pk_len_k] = ave_pk_len;
    } 
    return comp_res;
}

int main(int argc, char const *argv[])
{

    string line = "";
    fstream myfile(argv[1]);
    getline(myfile, line); // jump
    vector<string> line_split;
    map<string, map<string, vector<int>>> addr_m;

    
    while (getline(myfile, line))
    {
        split_by_space(line_split, line);
        string addr = line_split[0];
        int arr_time = stoi(line_split[5]);
        int pk_len = stoi(line_split[6]);
        // cout << "addr : " << addr << " arr_time: " << arr_time << " pk_len: " << pk_len << endl;
        if (addr_m.count(addr) == 0)
        {
            map<string, vector<int>> calcu_m;
            vector<int> ar_time_v;
            vector<int> pk_len_v;
            addr_m.insert(pair<string, map<string, vector<int>>>(addr, calcu_m));
            map<string, map<string, vector<int>>>::iterator iter = addr_m.find(addr);
            iter->second.insert(pair<string, vector<int>>(arrive_time_k, ar_time_v));
            iter->second.insert(pair<string, vector<int>>(package_len_k, pk_len_v));
        }
        addr_m.find(addr)->second.find(arrive_time_k)->second.push_back(arr_time);
        addr_m.find(addr)->second.find(package_len_k)->second.push_back(pk_len);
        // cout << line_split.size() << endl;  
        line_split.clear();
    }
    map<string, map<string, int>> comp_res= compute(addr_m);

    map<string, map<string, int>>::iterator iter;
    for (iter = begin(comp_res); iter != end(comp_res); iter++)
    {
        cout << iter->first << endl;
        string arr_t_k = iter->second.find(ave_arr_time_k)->first;
        int arr_t_v = iter->second.find(ave_arr_time_k)->second;
        string pk_len_k = iter->second.find(ave_pk_len_k)->first;
        int pk_len_v = iter->second.find(ave_pk_len_k)->second;
        cout << arr_t_k << ":" << arr_t_v << endl;
        cout << pk_len_k << ":" << pk_len_v << endl;
    }

    return 0;
}
