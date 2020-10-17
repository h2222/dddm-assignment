# coding=utf-8


def get_ave(arrt_ls, pk_len_ls):
    total_arrt = 0
    for i in range(len(arrt_ls) - 1, 0, -1):
        total_arrt += (arrt_ls[i] - arrt_ls[i-1])
    ave_arrt = total_arrt / (len(arrt_ls) - 1 + 1e-10)
    ave_pk_len = sum(pk_len_ls) / (len(pk_len_ls) + 1e-10)
    return ave_arrt, ave_pk_len


def test(path):
    f = open(path, 'r')
    f.readline()
    kvp = {}
    info_ls = []
    for line in f.readlines():
        line_ls = line.split(" ")
        in_addr = line_ls[0]
        in_port = line_ls[1]
        out_addr = line_ls[2]
        out_port = line_ls[3]
        arrt = int(line_ls[-2])
        pk_len = int(line_ls[-1])
        info = "{} {} {} {}".format(in_addr, in_port, out_addr, out_port)
        if info not in info_ls:
            info_ls.append(info)
        if info not in kvp:
            kvp[info] = {}
            kvp[info]["arrt_ls"] = []
            kvp[info]["kp_len_ls"] = []
        kvp[info]["arrt_ls"].append(arrt)
        kvp[info]["kp_len_ls"].append(pk_len)
    for info in info_ls:
        arrt_ls = kvp[info]["arrt_ls"]
        pk_len_ls = kvp[info]["kp_len_ls"]
        ave_arrt, ave_pk_len = get_ave(arrt_ls, pk_len_ls)
        if ave_arrt != 0:
            print("%.2f %.2f" % (ave_arrt, ave_pk_len))



if __name__ == "__main__":
    test(sys.argv[1])