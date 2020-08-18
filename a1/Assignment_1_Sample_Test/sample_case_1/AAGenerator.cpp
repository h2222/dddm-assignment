#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

vector<string> split(string& s, string& c)
{
	string::size_type pos1, pos2;
	pos1 = s.find(c);
	pos2 = 0;
	while(string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2-pos1));
	}
	pos1 = pos2 + c.size();
	pos2 = s.find(c, pos1);
	if (pos1 != s.length())
	{
		v.push_
	}
}




int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		string fname = argv[i];
		string att = "att";
		string que = "query";
		string acc = "acc";

		if (fname.find(att) != string::npos)
		{
			cout << fname << endl;	
			map<string, string> attm;
			fstream myfile(argv[i]);
			string line;
			while(getline(myfile, line))
			{
				char lc[line.length()];
				
				cout << line << endl;
				//attm.insert(pair<String, String>())
			}
		} 
		else if (fname.find(que) != string::npos)
		{
			cout << fname << endl;	
			map<string, string> quem;
			fstream myfile(argv[i]);
			string line;
			while(getline(myfile, line))
			{
				cout << line << endl;
			}

		}
		else if (fname.find(acc) != string::npos)
		{
			cout << fname << endl;	
			map<string, string> accm;
			fstream myfile(argv[i]);
			string line;
			while(getline(myfile, line))
			{
				cout << line << endl;
			}
		}
	}
	return 0;
}
