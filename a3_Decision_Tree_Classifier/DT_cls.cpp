#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include<set>  

using namespace std;

const string qlt_k = "quality";
vector<double> quality_label;

struct Node
{
    double current_threshold;
    string current_feature;
    struct Node* left_child;
    struct Node* right_child;
    Node(){}    
    Node(string feature, double threshold):current_feature(feature), current_threshold(threshold), left_child(nullptr), right_child(nullptr){}
};


double probability(const int& i, const int& total)
{
    return (double) i / (total + 1e-15);
}


double entropy(const double& prob)
{
    if (prob != 0)
    {
        return (double) (-1) * prob * log2(prob);
    }
    return 0;
}


double ENTROPY(const vector<double>& target_v)
{
    double res = 0;
    int total = target_v.size();
    for (double i : quality_label)
    {
        int ct =  count(begin(target_v), end(target_v), i);
        double p = probability(ct, total);
        double ent = entropy(p);
        res += ent;
    }
    return res;
}


void sort_with_index(vector<double>& v, vector<int>& origin_idx)
{
    int len = v.size();
    int min_idx, old_min_idx;
    int og_idx[len];
    for (int i = 0; i < len; i++)
    {
        og_idx[i] = i;
    }
    for (int i = 0; i < len; i++)
    {
        min_idx = i;
        old_min_idx = min_idx;
        for (int j = min_idx + 1; j < len; j++)
        {
            if (v[j] < v[min_idx]) 
            {
                min_idx = j;
            }
        }
        if (min_idx != old_min_idx)
        {
            og_idx[i] = min_idx;
            og_idx[min_idx] = i;
        }
        swap(v[i], v[min_idx]);
    }

    for (int i = 0; i < len; i++)
    {
        origin_idx.push_back(og_idx[i]);
    }
}


double GAINRATIO(const double& threshold, vector<double> value_v, vector<double> quality_v, vector<int>& origin_idx, int& split_point)
{
    int len = value_v.size();
    vector<double> new_qaul_v;
    sort_with_index(value_v, origin_idx);
    // sort quality vector by origin_idx
    for (int i = 0; i < len; i++)
    {
        int old_idx = origin_idx[i];
        new_qaul_v.push_back(quality_v[old_idx]);
    }
    vector<double>::iterator it = find(begin(value_v), end(value_v), threshold);
    int thresh_idx = distance(begin(value_v), it);
    int a_total = 0;
    vector<double> new_qual_v_a;
    vector<double> new_qual_v_b;
    for (int i = 0; i < len; i++)
    {
        if (value_v[i] <= threshold)
        {
            a_total ++;
        }
    }
    for (int i = 0; i < a_total; i++)
    {
        new_qual_v_a.push_back(new_qaul_v[i]);
    }
    for (int i = a_total; i < len; i++)
    {
        new_qual_v_b.push_back(new_qaul_v[i]);
    }

    split_point = a_total;

    double ent = ENTROPY(new_qaul_v);
    double a_ent = ENTROPY(new_qual_v_a);
    double b_ent = ENTROPY(new_qual_v_b);

    double p_a =  probability(a_total, len);
    double p_b = probability(len - a_total, len);
    
    double gain = ent - p_a * a_ent - p_b * b_ent;
    double split_info = entropy(p_a) + entropy(p_b);
    double gain_ratio = gain / (split_info + 1e-15);
    // cout << "ent: " << ent << " a_ent: " << a_ent << " b_ent: " << b_ent << " p_a: " << p_a << " p_b: " << p_b << endl;
    // cout << " gain: " << gain << " split info: " << split_info << " gain ratio: " << gain_ratio <<endl;
    return gain_ratio;
}


void C45DT(map<string, vector<double>> table,  vector<string> head, struct Node* current_node_p, int epoch, string direction)
{
    set<double> qual_kinds(begin(table[qlt_k]), end(table[qlt_k]));
    if ((qual_kinds.size() <= 1) || (head.size() == 1 && head[0] == qlt_k))
    {
        cout << "stop position: " << direction << ", data: ";
        for (double i : table[qlt_k])
        {
            cout << i << " ";
        }
        cout << endl;
        return;
    }
    double best_threshold;
    double total_max_gain_ratio = - 0xffffff;
    string best_feature;
    vector<int> best_origin_idx;
    int best_split_point;
    for (string h_name : head)
    {   
        if (h_name == qlt_k)
        {
            break;
        }
        // cout << h_name << endl;
        vector<double> curr_v1 = table[h_name];
        sort(begin(curr_v1), end(curr_v1));
        curr_v1.erase(unique(begin(curr_v1), end(curr_v1)), end(curr_v1)); // 去重
        double better_threshold;
        double max_gain_ratio = - 0xffffff;
        int better_split_point;
        vector<int> origin_idx;
        for (double threshold : curr_v1)
        {
            int split_point = 0;
            // cout << "threshold: " << threshold << endl;
            double gain_ratio = GAINRATIO(threshold, table[h_name], table[qlt_k], origin_idx, split_point);
            // cout << " threshold: " << threshold << "gain ratio: "<< gain_ratio << endl; 
            if (max_gain_ratio < gain_ratio)
            {
                max_gain_ratio = gain_ratio;
                better_threshold = threshold;
                better_split_point = split_point;
            }
        }
        if (total_max_gain_ratio < max_gain_ratio)
        {
            best_threshold = better_threshold;
            best_feature = h_name;
            total_max_gain_ratio = max_gain_ratio;
            best_split_point = better_split_point;
            best_origin_idx = origin_idx;
        }
        cout << "better threshold: " << better_threshold << " max gain ratio: "<< max_gain_ratio << endl;
    }
    cout << "epoch: "<< epoch <<" direction: "<< direction << " ---best feature: " << best_feature << " best threshold: " << best_threshold << "best split point: " << best_split_point <<"---"<< endl;

    current_node_p = new Node(best_feature, best_threshold);
    struct Node* lnode;
    struct Node* rnode;
    current_node_p->left_child = lnode;
    current_node_p->right_child = rnode;

    vector<string> new_head;
    for (int i = 0; i < head.size(); i++)
    {
        if (head[i] == best_feature) 
        {
            continue;
        }
        new_head.push_back(head[i]);
    }

    map<string, vector<double>> new_table_left;
    map<string, vector<double>> new_table_right;
    int len = table[best_feature].size();
    // <= threshold
    for (string h_name : new_head)
    {
        for (int i = 0; i < best_split_point; i++)
        {
            new_table_left[h_name].push_back(table[h_name][best_origin_idx[i]]);
        }
    }
    // > threshold
    for (string h_name : new_head)
    {
        for (int i = best_split_point; i < len; i++)
        {
            new_table_right[h_name].push_back(table[h_name][best_origin_idx[i]]);
        }
    }

    // cursion: left first, then right
    epoch++;
    string left_sign = best_feature + "-Left";
    string right_sign = best_feature + "-Right";
    C45DT(new_table_left, new_head, current_node_p->left_child, epoch, left_sign);
    C45DT(new_table_right, new_head, current_node_p->right_child , epoch, right_sign);
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
    struct Node* init_node_p;
    vector<string> head;
    map<string, vector<double>> table;
    getline(wine_f, line);
    split(head, ",", line);
    int head_index = head.size();
    for (string s : head)
    {
        vector<double> values;
        table[s] = values;
    }
    while (getline(wine_f, line))
    {
        vector<string> values;
        split(values, ",", line);
        for (int i = 0; i < head_index; i++)
        {
            table[head[i]].push_back(stod(values[i]));
        }
    }
    for (double i = 0; i <= 10; i++)
    {
        quality_label.push_back(i);
    }
    int epoch = 0;
    C45DT(table, head, init_node_p, epoch, "root_init");
    return 0;
}