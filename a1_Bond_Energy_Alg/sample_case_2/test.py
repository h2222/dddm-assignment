import math



f = open('acc_2.txt', 'r')


for i in f.readlines()[1:]:
    i = i.split(' ')

    ii = i[1:]
    q = i[0]
    iii = list(map(int, ii))
    print("q: %s sum: %d" % (q, sum(iii)))



a = (283*283) / math.sqrt((295+283) * (283+271))

print(a)




