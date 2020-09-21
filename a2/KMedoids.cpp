#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

template<typename T>
void split(vector<T>& st_result, string& s, string& text)
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

int main(int argc, char const *argv[])
{

    string line = "";
    fstream myfile(argv[1]);
    getline(myfile, line); // jump
    vector<string> line_split;
    while (getline(myfile, line))
    {
        split_by_space(line_split, line);
        cout << line_split.size() << endl;  
        // for (string s : line_split)
        // {
        //     cout << s.length() << " | ";
        // }
        // cout << endl;
        line_split.clear();
    }
    return 0;
}
