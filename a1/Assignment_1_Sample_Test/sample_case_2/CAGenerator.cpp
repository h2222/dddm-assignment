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
    // fix matrix order 
    // int AA_p_c[size][size];
    // for (int i = 0; i < size; i++)
    // {
    //     for (int j = 0; j < size; j++)
    //     {
    //         AA_p_c[i][j] = AA_p[i][j];
    //         // cout << AA_p_c[i][j] << "    ";
    //     }
    // }  
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
    cout << "best_position: " << best_loc << " loc contribution: " << best_loc_cont << endl;
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


int** transfer_v3(int** AA_p_1c, int* temp, int size)
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
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << CA_p[i][j] << "    ";
        }
        cout << endl;
    }
    return CA_p;
}


// best: 2   index: 3 
int* transfer_v2(int* temp, const int best_position, const int index, const int size)
{
    int* res = new int [size];
    if (best_position == index)
    {
        res = temp;
        return res;   
    }
    int target = temp[index];
    res[best_position] = target;
    // cout << res[best_position] << endl;
    int i = 0;
    int p = 0;
    int q = 0;
    for (; i < size+1; i++)
    {
        if (i == best_position)
        {   
            // cout << res[q] << endl;
            q++;
            // cout << "p: " << p << ", q: " << q << endl; 
            continue;
        } 
        else if (i == index)
        {
            res[q] = temp[p];
            p++;
            // cout << res[q] << endl;
            // cout << "?" << endl;
            // cout << "p: " << p << ", q: " << q << endl; 
            continue;
        }
        if (p != index)
        {
            res[q] = temp[p];
            // cout << res[q] << endl;
        }
        p++;
        q++;
        // cout << "p: " << p << ", q: " << q << endl; 
    }
    return res;
}



void generator(int** AA_p, int** AA_p_c, int size)
{
    // int** CA_p = new int* [size];
    // map<int, int> idx_loc;
    // CA_p[0] = AA_p[0];
    // CA_p[size - 1] = AA_p[size - 1];
    // CA_p[1] = AA_p[1];
    // CA_p[2] = AA_p[2];    
    int index = 3;
    int* temp = new int [size]; //= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};    
    for (int i = 0; i < size; i++)
    {
        temp[i] = i;
        cout << temp[i] << "    ";
    }
    // origin AA
    cout << endl;
    cout << "initailization" << endl;

    while (index < size - 1)
    {
        int* loc = new int[index+1];
        for (int i = 1; i <= index - 1; i++)
        {
            cout << "(i-1): " << i - 1 << "    (i): " << i << "    (index): " << index << endl;
            int b1 = bond(AA_p, i - 1, index, size);
            cout << "bond[i - 1, index]: " << b1 << endl;
            int b2 = bond(AA_p, index, i, size);
            cout << "bond[index, i]: " << b2 << endl;
            int b3 = bond(AA_p, i - 1, i, size);
            cout << "bond[i - 1, i]: " << b3 << endl;
            int cont1 = 2 * b1 + 2 * b2 - 2 * b3;
            cout << "2 x bond(i-1, index) + 2 x bond(index, i) - 2 x bond(i-1, i) == " << cont1 << endl;
            cout << "------------------------------------------------------------" << endl;
            loc[i] = cont1;
        }
        cout << "(index-1): " << index - 1 << "    (index): " << index << "    (index+1): " << index + 1 << endl;
        int bb1 = bond(AA_p, index - 1, index, size);
        cout << "bond[index - 1, index]: " << bb1 << endl;
        // int bb2 = bond(AA_p, index, index + 1, size);
        int bb2 = bond(AA_p, index, 0, size);
        cout << "bond[index, index + 1]: " << bb2 << endl;
        // int bb3 = bond(AA_p, index - 1, index + 1, size);
        int bb3 = bond(AA_p, index - 1, 0, size);
        cout << "bond[index - 1, index + 1]: " << bb3 << endl;
        int cont2 = 2 * bb1 + 2 * bb2 - 2 * bb3;
        cout << "2 x bond(index-1, index) + 2 x bond(index, index+1) - 2 x bond(index-1, index+i) == " << cont2 << endl;
        loc[index] = cont2;
        int best_position = arg_max_index(loc, index);  // GET THE BEST POSITION OF THE CA COLUMN.
        // idx_loc.insert(pair<int, int>(index, best_position));
        temp = transfer_v2(temp, best_position, index, size);
        cout << "ORDER" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << temp[i] << "    ";
        }
        // cout << endl;
        // cout << "origin" << endl;
        // for (int i = 0; i < size; i++)
        // {
        //     for (int j = 0; j < size; j++)
        //     {
        //         cout << AA_p_c[i][j] << "    ";
        //     }
        //     cout << endl;
        // }
        cout << endl;
        AA_p = transfer_v3(AA_p_c, temp, size);
        // transfer(CA_p, AA_p, index, best_position, size);
        index++;
        cout << endl;
        cout << endl;
    }
    // transpose
    // for (int i = 0; i < size; i++)
    // {
    //     for(int j = 0; j < i; j++)
    //     {
    //         int temp3 = CA_p[i][j];
    //         CA_p[i][j] = CA_p[j][i];
    //         CA_p[j][i] = temp3;
    //     }
    // }
	// map<int, int>::iterator iter;
    // int** res = new int* [size];
    // res[0] = CA_p[0];
    // res[size - 1] = CA_p[size - 1];
    // res[1] = CA_p[1];
    // res[2] = CA_p[2];  
    // for(iter = begin(idx_loc); iter != end(idx_loc); iter++)
    // {
    //     int index = iter->first;
    //     int best_position = iter->second;
    //     transfer(res, CA_p, index, best_position, size);
    // }



    // // cout << "      A1     A2     A3     A4" << endl;
    // for (int i = 1; i < size - 1; i++)
    // {
    //     // cout <<"A"<<i<<"    ";
    //     for (int j = 1; j < size - 1; j++)
    //     {
    //         cout << res[i][j] << " ";
    //     }
    //     cout << endl;
    // }
}


int main(int argc, char* argv[])
{
    fstream myfile(argv[1]);
    string line;
    vector<string> line_save;
    int size = 0;

    // while (getline(myfile, line))
    // {
    //     cout << line << endl;
    // }


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
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << AA_p_c[i][j] << "     ";
        }
        cout << endl;
    }
    generator(AA_p, AA_p_c, size);


    // int temp[size] = {0, 16, 10, 6, 9, 7, 3, 4, 14, 5, 11, 15, 17, 13, 1, 19, 8, 2, 18, 12, 20};
    // int temp[size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    // int* tempp = temp;
    // tempp = transfer_v2(tempp, 2, 5, 21);
    // for (int i = 0 ; i < size; i++)
    // {
    //     cout << tempp[i] << " ";
    // }
    // cout << endl;
    return 0;
}