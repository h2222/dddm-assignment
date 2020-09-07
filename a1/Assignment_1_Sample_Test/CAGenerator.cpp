#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>


using namespace std;


template <typename T>
void split(vector<T>& st_result, const string& s, const string& text)
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
        if (loc[i] > best_loc_cont)
        {
            best_loc_cont = loc[i];
            best_loc = i;
        }
    }
    return best_loc;
}


int** transfer(int** AA_p_1c, int* temp, int size)
{
    int AA_p_2c [size][size];
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            AA_p_2c[i][j] = AA_p_1c[i][j];
        }
    }
    int** CA_p = new int* [size];
    for (int i = 0; i < size; i++)
    {
        CA_p[i] = new int [size];
        for (int j = 0; j < size; j++)
        {
            CA_p[i][j] = AA_p_2c[temp[i]][j];
        }
    }
    for (int i = 0; i < size; i++)
    {
        for(int j = 0; j < i; j++)
        {
            int temp3 = CA_p[i][j];
            CA_p[i][j] = CA_p[j][i];
            CA_p[j][i] = temp3;
        }
    }
    return CA_p;
}


// best: 2   index: 3 
int* get_position_list(int* temp, const int best_position, const int index, const int size)
{
    int* res = new int [size];
    if (best_position == index)
    {
        res = temp;
        return res;   
    }
    int target = temp[index];
    res[best_position] = target;
    int i = 0;
    int p = 0;
    int q = 0;
    for (; i < size+1; i++)
    {
        if (i == best_position)
        {   
            q++;
            continue;
        } 
        else if (i == index)
        {
            res[q] = temp[p];
            p++;
            continue;
        }
        if (p != index)
        {
            res[q] = temp[p];
        }
        p++;
        q++;
    }
    return res;
}



void generator(int** AA_p, int** AA_p_c, int size)
{
    int index = 3;
    int* temp = new int [size]; //= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};    
    for (int i = 0; i < size; i++)
    {
        temp[i] = i;
    }
    while (index < size - 1)
    {
        int* loc = new int[index+1];
        for (int i = 1; i <= index - 1; i++)
        {
            int b1 = bond(AA_p, i - 1, index, size);
            int b2 = bond(AA_p, index, i, size);
            int b3 = bond(AA_p, i - 1, i, size);
            int cont1 = 2 * b1 + 2 * b2 - 2 * b3;
            loc[i] = cont1;
        }
        int bb1 = bond(AA_p, index - 1, index, size);
        int bb2 = bond(AA_p, index, 0, size);
        int bb3 = bond(AA_p, index - 1, 0, size);
        int cont2 = 2 * bb1 + 2 * bb2 - 2 * bb3;
        loc[index] = cont2;
        int best_position = arg_max_index(loc, index);  // GET THE BEST POSITION OF THE CA COLUMN.
        temp = get_position_list(temp, best_position, index, size);
        AA_p = transfer(AA_p_c, temp, size);
        index++;
    }
    // print step
    int** CA_p = new int* [size];
    cout << "      ";
    for (int i = 1; i < size - 1 ; i++)
    {
        cout << "A"<< temp[i] << "     ";
    }
    cout << endl;
    for (int i = 1; i < size - 1; i++)
    {
        CA_p[i] = AA_p[temp[i]];
        cout << "A" << temp[i] << "    "; 
        for (int j = 1; j < size - 1; j++)
        {
            cout << CA_p[i][j] << " ";
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
    int** AA_p_c = new int* [size];
    int** AA_p = new int* [size]; 
    for (int i = 0; i < size; i++)
    {
        AA_p_c[i] = new int [size];
        AA_p[i] = new int [size];
        for (int j = 0; j < size; j++)
        {
            if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
            {
                AA_p[i][j] = 0;
                AA_p_c[i][j] = 0;
            }
            else
            {
                AA_p[i][j] = stoi(line_save[(i - 1) * (size - 2) + (j - 1)]); 
                AA_p_c[i][j] = stoi(line_save[(i - 1) * (size - 2) + (j - 1)]); 
            }
        }
    }
    generator(AA_p, AA_p_c, size);
    return 0;
}