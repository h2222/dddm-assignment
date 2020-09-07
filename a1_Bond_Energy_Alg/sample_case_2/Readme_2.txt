We will compile and run your code with the following commands:
g++ -std=c++11 AAGenerator.cpp -o runAA
./runAA att_2.txt query_2.txt acc_2.txt > aa_2.txt

g++ -std=c++11 CAGenerator.cpp -o runCA
./runCA aa_2.txt > ca_2.txt