import sys



def test(path):
    f = open(path, 'r')
    f.readline()
    arrt_ls = []
    pk_len_ls = []
    addr = set()
    for line in f.readlines():
        line_ls = line.split(" ")
        in_addr = line_ls[0]
        in_port = line_ls[1]
        out_addr = line_ls[2]
        out_port = line_ls[3]
        arrt = int(line_ls[-2])
        pk_len = int(line_ls[-1])
        
        target_addr = '203.48.9.248'
        target_port =  '1500' #'54194'
        target_pkg = 1500
        if pk_len == target_pkg:
            print('good')
            addr.add("{} {} {} {}".format(in_addr, in_port, out_addr, out_port))
            arrt_ls.append(int(arrt))
            pk_len_ls.append(int(pk_len))
        
    print(addr)
    print(arrt_ls)
    print(pk_len_ls.__len__())

    total_arrt = 0
    for i in range(len(arrt_ls) - 1, 0, -1):
        total_arrt += (arrt_ls[i] - arrt_ls[i-1])
    ave_arrt = total_arrt / (len(arrt_ls) - 1 + 1e-10)

    ave_pk_len = sum(pk_len_ls) / (len(pk_len_ls) + 1e-10)

    print("%.2f %.2f" % (ave_arrt, ave_pk_len))









# ave_trans_time = ((115363 - 115350) + (115350 - 115314)) / (3 - 1)
# ave_time = (40 + 40 + 40) / 3
# print(ave_trans_time)
# print(ave_time)




if __name__ == "__main__":
    test(sys.argv[1])