


  0 a b c d e
0 0 1 2 3 4 5
b
c
d
e
f



def editDsitance(str1, str2):
    s1_len = len(str1)
    s2_len = len(str2)
    dp = [[0 for _ in range(s2_len+1)] for _ in range(s1_len+1)]


    for i in range(s1_len+1):
        for j in range(s2_len+1):
            if i == 0:
            else





if __name__ == "__main__":
    str1 = "abcde"
    str2 = "xxxxx"
