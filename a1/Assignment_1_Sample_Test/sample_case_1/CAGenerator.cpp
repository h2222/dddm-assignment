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


void print_square_matrix(int** matrix_p, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << matrix_p[i][j] << " ";
        }
        cout << endl;
    }
}



int bond(int** AA_p, int a, int b, int size)
{
    int res = 0;
    for (int i = 0; i < size; i++)
    {
        // cout << "[i, a] -> " << i << " " << a << ", " << "[i, b] -> " << i << " " << b << endl;
        // cout << AA_p[i][a] << " * " << AA_p[i][b] << endl;
        res += AA_p[i][a] * AA_p[i][b];
    }
    // cout << endl;
    // cout << endl;
    // cout << res << endl;
    return res;
}


int arg_max_index(int* loc, int index)
{
    cout << " loc list " << endl;
    int best_loc = 0;
    int best_loc_cont = 1<<31;
    for (int i = 1; i <= index; i++)
    {
        if (loc[i] > best_loc_cont)
        {
            best_loc_cont = loc[i];
            best_loc = i;
        }
    }
    cout << "best_position: " << best_loc << " loc contribution: " << best_loc_cont << endl;
    cout << endl;
    cout << endl;
}


void generator(int** AA_p, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << AA_p[i][j] << " ";
        }
        cout << endl;
    }
    int** CA_p = new int* [size];

    CA_p[1] = AA_p[1];
    CA_p[2] = AA_p[2];
    int index = 3;
    while (index < size - 1)
    {
        int* loc = new int[index+1];
        for (int i = 1; i <= index - 1; i++)
        {
            cout << "(i-1): " << i - 1 << " (i): " << i << " (index): " << index << endl;
            int a = bond(AA_p, i - 1, index, size);
            int b = bond(AA_p, index, i, size);
            int c = bond(AA_p, i - 1, i, size);
            cout << " a: " << a << " b: " << b << " c: " << c << endl;
            int cont = 2 * a + 2 * b - 2 * c;
            loc[i] = cont;
            cout << cont << endl;
        }
        cout << "(index-1): " << index - 1 << " (index): " << index << " (index+1): " << index + 1 << endl;
        int cont = 2 * bond(AA_p, index - 1, index, size) + 2 * bond(AA_p, index, index + 1, size) - 2 * bond(AA_p, index - 1, index + 1, size);
        cout << cont << endl; 
        loc[index] = cont;
        int best_position = arg_max_index(loc, index);  // GET THE BEST POSITION OF THE CA COLUMN.
        index++;
    }

    for (int i = 0; i < size; i++)
    {
        cout << *CA_p[2] << " ";
        CA_p[2]++;
    }   
    cout << endl; 
    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //     {
    //         cout << "i: " << i << " j:" << j << endl;
    //         CA_a = AA_p[0];
    //         CA_b = AA_p[2];  

    //     }
    // }
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

    print_square_matrix(AA_p, size);



    // int res = bond(AA_p, 1, 2, size);
    // cout << "res: " << res << endl;
    generator(AA_p, size);

    // int (*line_p)[y_size] = AA_p;

    // for (int i = 0; i < y_size; i++)
    // {
    //     cout << *line_p[0] << " "; // print line
    //     line_p++;   
    // }
    return 0;
}