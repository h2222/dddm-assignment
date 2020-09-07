We will compile and run your code with the following commands:
g++ -std=c++11 AAGenerator.cpp -o runAA
./runAA att_1.txt query_1.txt acc_1.txt > aa_1.txt

g++ -std=c++11 CAGenerator.cpp -o runCA
./runCA aa_1.txt > ca_1.txt