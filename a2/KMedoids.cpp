#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;

const string arrive_time_k = "arrive_time";
const string package_len_k = "package_len";
const string ave_arr_time_k = "ave_arrive_time";
const string ave_pk_len_k = "ave_package_len";
const string cen_idx_k = "cen_idx";
const string total_cost_k = "total_cost_k";
const string min_idx_ls_k = "min_idx_ls_k";
const string min_ls_k = "min_ls_k";

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


map<string, map<string, float>> compute(map<string, map<string, vector<int>>>& addr_m)
{
    map<string, map<string, float>> comp_res;
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

/* Mannhaton distance
*/
float comp_dist(float* a, float* b)
{
    float res = 0.;
    for (int i = 0; i < 2; i++)
    {
        res += abs(a[i] - b[i]);
    }
    return res; 
}

/* implementing argmin
*/
int argmin(float* f_list, int k)
{
    int res  = 0.;
    for (int i = 0; i < k; i++)
    {
        if (f_list[res] > f_list[i])
        {
            res = i;
        }
    }
    return res;
}


/* implementing min
*/
int min(float* f_list, int k)
{
    float res_f = 0xffffff;
    for (int i = 0; i < k; i++)
    {
        if (res_f > f_list[i])
        {
            res_f = f_list[i];
        }
    }
    int res_i = (int) (res_f * 100); // time 100 for get 2 last reminder
    return res_i;
}

/* computing eclud distance between each feature values 
*/
map<string, int*> comp_min_dist(float** data, float** med_obj, int k, int len)
{
    float** dist_mat = new float* [len];
    int* min_idx_ls = new int [len];
    int* min_ls = new int [len];
    map<string, int*> res;
    for (int i = 0; i < len; i++)
    {
        dist_mat[i] = new float[k];
        for (int j = 0; j < k; j++)
        {
            dist_mat[i][j] = comp_dist(data[i], med_obj[j]);
            // cout << dist_mat[i][j] << " ";
        }
        // cout << endl;
        min_idx_ls[i] = argmin(dist_mat[i], k);
        min_ls[i] = min(dist_mat[i], k);
    }
    res[min_idx_ls_k] = min_idx_ls;
    res[min_ls_k] = min_ls;
    // cout << len << endl;
    // cout << k << endl;
    return res;
}


// k medoids clustering alg

void assment(float** data, map<string, vector<int>>& cur_medoids, int k, int len)
{  
    float** med_obj = new float* [k];
    for (int i = 0; i < k; i++)
    {
        med_obj[i] = new float[2];
        int idx = cur_medoids[cen_idx_k][i]; // medoids index
        med_obj[i] = data[idx];
    }
    map<string, int*> min_idx_and_value = comp_min_dist(data, med_obj, k, len); // min distance for each row of data
    int* min_idx_ls = min_idx_and_value[min_idx_ls_k];
    int* min_ls = min_idx_and_value[min_ls_k];

    // total cost
    vector<int> total_c;
    float total_cost = 0;
    for (int i = 0; i < len; i++)
    {
        total_cost += min_ls[i];
    }
    total_c.push_back(total_cost);
    cur_medoids[total_cost_k] = total_c;
    
    for (int i = 0; i < k; i++)
    {
        vector<int> cluster_idx;
        for (int j = 0; j < len; j++)
        {
            if (i == min_idx_ls[j])
            {
                cluster_idx.push_back(j);
            }
            cur_medoids[to_string(i)] = cluster_idx;
        }
    }
}


/* convert vector to set
*/
set<int> convert_to_set(vector<int> v)
{
    set<int> s;
    for (int i : v)
    {
        s.insert(i);
    }
    return s;
}


/* copy map
 */
 void map_copy(map<string, vector<int>>& m_from, map<string, vector<int>>& m_to, vector<string> key_vec)
 {
    for (string key : key_vec)
    {
        m_to[key] = m_from[key];
    }
 }


int k_medoids_cluster(float** data, int len, int k, vector<int> med_idx)
{
    map<string, vector<int>> cur_medoids;
    map<string, vector<int>> old_medoids;
    vector<int> old_cen_idx;
    vector<string> key_vec;
    set<int> cur_med_idx_s;
    set<int> old_med_idx_s;
    int iter_counter = 1;
    cur_medoids[cen_idx_k] = med_idx;
    assment(data, cur_medoids, k, len);
    old_medoids[cen_idx_k] = old_cen_idx;


    cur_med_idx_s = convert_to_set(cur_medoids[cen_idx_k]);
    old_med_idx_s = convert_to_set(old_medoids[cen_idx_k]);
    map<string, vector<int>>::iterator iter;
    for (iter = begin(cur_medoids); iter != end(cur_medoids); iter++)
    {
        key_vec.push_back(iter->first);
    }
    
    cout << "origin" << endl;
    for (int i : cur_medoids[cen_idx_k])
    {
        cout << i << " ";
    }
    
    cout << endl;


    while (cur_med_idx_s != old_med_idx_s)
    {
        // for (int i : cur_med_idx_s)
        // {
        //     cout << i << " ";
        // }
        // cout << endl;

        // for (int i : old_med_idx_s)
        // {
        //     cout << i << " ";
        // }
        cout << endl;
        cout << "---------  iteration count: " << iter_counter++ << "  ----------" << endl;
        cout << " before iterating, the total cost is :" << cur_medoids[total_cost_k][0] << endl;
        map<string, vector<int>> best_medoids;
        map_copy(cur_medoids, best_medoids, key_vec);
        map_copy(cur_medoids, old_medoids, key_vec);
        for (int i = 0; i < len; i++)
        {
            for (int j = 0; j < k; j++)
            {
                if (i != j)
                {
                    map<string, vector<int>> temp_medoids;
                    map_copy(cur_medoids, temp_medoids, key_vec);
                    temp_medoids[cen_idx_k][j] = i;
                    // for (int i : temp_medoids[cen_idx_k])
                    // {
                    //     cout << i << " ";
                    // }
                    // cout << endl;
                    assment(data, temp_medoids, k, len);
                    if (best_medoids[total_cost_k][0] > temp_medoids[total_cost_k][0])
                    {
                        map_copy(temp_medoids, best_medoids, key_vec);
                    }
                }
            }
        }
        for (int i : best_medoids[cen_idx_k])
        {
            cout << i << " ";
        }
        cout << endl;
        map_copy(best_medoids, cur_medoids, key_vec);
        cur_med_idx_s = convert_to_set(cur_medoids[cen_idx_k]);
        old_med_idx_s = convert_to_set(old_medoids[cen_idx_k]);
        cout << "current total cost is : " << cur_medoids[total_cost_k][0] << endl;
    }
    return 0;
}




int main(int argc, char const *argv[])
{
    string line = "";
    fstream myfile(argv[1]);
    ofstream write("Flow.txt", ios::out);
    getline(myfile, line); // jump
    vector<string> line_split;
    vector<string> addr_ls;
    map<string, map<string, vector<int>>> addr_m;
    while (getline(myfile, line))
    {
        split_by_space(line_split, line);
        string in_addr = line_split[0];
        string in_p = line_split[1];
        string out_addr = line_split[2];
        string out_p = line_split[3];
        string addr = in_addr + " " + in_p + " " + out_addr + " " + out_p;
        // cout << addr << endl;
        int arr_time = stoi(line_split[5]);
        int pk_len = stoi(line_split[6]);
        // cout << "addr : " << addr << " arr_time: " << arr_time << " pk_len: " << pk_len << endl;
        if (addr_m.count(addr) == 0)
        {
            map<string, vector<int>> calcu_m;
            vector<int> ar_time_v;
            vector<int> pk_len_v;
            addr_ls.push_back(addr);
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
    map<string, map<string, float>> comp_res= compute(addr_m);
    int index = 0;
    vector<float> arrt_t_v;
    vector<float> pk_len_v;
    for (int i = 0; i < addr_ls.size(); i++)
    {
        string addr = addr_ls[i];
        float ave_arr_time = comp_res.find(addr)->second.find(ave_arr_time_k)->second;
        float ave_pk_length = comp_res.find(addr)->second.find(ave_pk_len_k)->second;
        if (ave_arr_time != 0)
        {
            write << index << " " << fixed << setprecision(2) << ave_arr_time << " " << ave_pk_length << endl;
            float temp_arrt = round(ave_arr_time * 100) / 100;
            float temp_pk_len = round(ave_pk_length * 100) / 100;
            arrt_t_v.push_back(temp_arrt);
            pk_len_v.push_back(temp_pk_len);
            // arrt_t_v.push_back
            index ++;
        }
    }
    // k - mediods implement
    int len = arrt_t_v.size();
    float** data = new float* [len]; // data matrix
    for (int i = 0; i < len; i++)
    {
        data[i] = new float [2];
        data[i][0] = arrt_t_v[i];
        data[i][1] = pk_len_v[i];
    } 
    // load k and init medoids index
    fstream myfile2(argv[2]);
    int k; // k
    vector<string> temp_med_idx;
    vector<int> med_idx;
    getline(myfile2, line);
    k =  stoi(line);
    getline(myfile2, line);
    split_by_space(temp_med_idx, line);
    for (string i : temp_med_idx)
    {
        med_idx.push_back(stoi(i));
    }
    k_medoids_cluster(data, len, k, med_idx);
    return 0;
}
