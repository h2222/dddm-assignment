from collections import OrderedDict



def ave_arrt_and_pkl(path):
    f = open(path, 'r')
    f.readline()
    store = OrderedDict()
    for line in f.readlines():
        line_ls = line.split(" ")
        arrt = int(line_ls[-2])
        pkl = int(line_ls[-1])
        addr = line_ls[0]

        if addr not in store:
            store[addr] = {}
            store[addr]["arrt"] = []
            store[addr]["pkl"] = []
        store[addr]["arrt"].append(arrt)
        store[addr]["pkl"].append(pkl)
    

    res = OrderedDict()
    i = 0
    for addr in store:
        len_arrt = len(store[addr]["arrt"])
        arrt_ls = store[addr]["arrt"]
        temp = 0
        for j in range(len(arrt_ls)-1, 0, -1):
            temp += (arrt_ls[j] - arrt_ls[j-1])
        ave_arrt = temp / (len_arrt - 1 + 1e-15)
        ave_pkl = sum(store[addr]["pkl"]) / (len_arrt + 1e-15)
        
        if ave_arrt != 0:
            print("{} {} {:.2f} {:.2f}".format(i, addr, ave_arrt, ave_pkl))
            i += 1


if __name__ == "__main__":
    ave_arrt_and_pkl('test.txt')