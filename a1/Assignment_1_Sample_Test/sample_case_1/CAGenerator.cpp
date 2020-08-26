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


int main(int argc, char* argv[])
{
    fstream myfile(argv[1]);
    string line;
    vector<string> line_save;
    
    int y_size = 0;
    int x_size = 0;
    while(getline(myfile, line))
    {
        split(line_save, " ", line);
        if (y_size == 0)
        {
            x_size = line_save.size();   
        }
        y_size++;        
    }
    cout << x_size << " " << y_size << endl;
    int AA_p[x_size][y_size];
    for (int i = 0; i < x_size; i++)
    {
        for (int j = 0; j < y_size; j++)
        {
            AA_p[i][j] = stoi(line_save[ i * x_size +j]); 
        }
    }

    int (*line_p)[y_size] = AA_p;

    for (int i = 0; i < y_size; i++)
    {
        cout << *line_p[0] << " "; // print line
        line_p++;   
    }
    return 0;
}