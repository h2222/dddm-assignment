#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
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


int bond(int** AA_p, int a, int b, int size)
{
    int res = 0;
    for (int i = 0; i < size; i++)
    {
        res += AA_p[i][a] * AA_p[i][b];
    }
    return res;
}


int arg_max_index(int* loc, int index)
{
    int best_loc = 0;
    int best_loc_cont = 1<<31;
    for (int i = 1; i <= index; i++)
    {
        if (loc[i] >= best_loc_cont)
        {
            best_loc_cont = loc[i];
            best_loc = i;
        }
    }
    // cout << "best_position: " << best_loc << " loc contribution: " << best_loc_cont << endl;
    return best_loc;
}


void transfer(int** CA_p, int** AA_p, int index, int best_position, int size)
{
    for (int j = index; j > best_position; j--)
        {
            CA_p[j] = CA_p[j - 1];
            int* temp = CA_p[j - 1];
        }
        CA_p[best_position] = AA_p[index];
        int* temp2 = AA_p[index];
}


void generator(int** AA_p, int size)
{
    int** CA_p = new int* [size];
    map<int, int> idx_loc;
    CA_p[0] = AA_p[0];
    CA_p[size - 1] = AA_p[size - 1];
    CA_p[1] = AA_p[1];
    CA_p[2] = AA_p[2];    
    int index = 3;
    while (index < size - 1)
    {
        int* loc = new int[index+1];
        for (int i = 1; i <= index - 1; i++)
        {
            // cout << "(i-1): " << i - 1 << " (i): " << i << " (index): " << index << endl;
            int cont = 2 * bond(AA_p, i - 1, index, size) + 2 * bond(AA_p, index, i, size) - 2 * bond(AA_p, i - 1, i, size);
            loc[i] = cont;
        }
        // cout << "(index-1): " << index - 1 << " (index): " << index << " (index+1): " << index + 1 << endl;
        int cont = 2 * bond(AA_p, index - 1, index, size) + 2 * bond(AA_p, index, index + 1, size) - 2 * bond(AA_p, index - 1, index + 1, size);
        loc[index] = cont;
        int best_position = arg_max_index(loc, index);  // GET THE BEST POSITION OF THE CA COLUMN.
        idx_loc.insert(pair<int, int>(index, best_position));
        transfer(CA_p, AA_p, index, best_position, size);
        index++;
    }
    // transpose
    for (int i = 0; i < size; i++)
    {
        for(int j = 0; j < i; j++)
        {
            int temp3 = CA_p[i][j];
            CA_p[i][j] = CA_p[j][i];
            CA_p[j][i] = temp3;
        }
    }
	map<int, int>::iterator iter;
    int** res = new int* [size];
    res[0] = CA_p[0];
    res[size - 1] = CA_p[size - 1];
    res[1] = CA_p[1];
    res[2] = CA_p[2];  
    for(iter = begin(idx_loc); iter != end(idx_loc); iter++)
    {
        int index = iter->first;
        int best_position = iter->second;
        transfer(res, CA_p, index, best_position, size);
    }
    for (int i = 1; i < size - 1; i++)
    {
        for (int j = 1; j < size - 1; j++)
        {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }
}


int main(int argc, char* argv[])
{
    fstream myfile(argv[1]);
    string line;
    vector<string> line_save;
    int size = 0;
    while (getline(myfile, line))
    {
        split(line_save, " ", line);
        if (size == 0)
        {
            size = line_save.size();   
        }
    }
    size += 2;
    int** AA_p = new int* [size]; 
    for (int i = 0; i < size; i++)
    {
        AA_p[i] = new int [size];
        for (int j = 0; j < size; j++)
        {
            if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
            {
                AA_p[i][j] = 0;
            }
            else
            {
                AA_p[i][j] = stoi(line_save[ (i - 1) * (size - 2) + (j - 1)]); 
            }
        }
    }
    generator(AA_p, size);
    return 0;
}