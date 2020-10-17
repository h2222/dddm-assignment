# coding=utf-8
import sys
import numpy as np



def load_data(path):
    f = open(path, 'r')
    label = [i for i in range(0, 11)]
    data = []
    for i, line in enumerate(f.readlines()):
        if i == 0:
            continue
        line2 = line.replace('\n', '').split(',')
        line3 = map(float, line2)
        data.append(list(line3))
    return data, label


def calcu_info_entropy(dataset):
    num = len(dataset)
    entropy = 0.0
    label_count = {}
    for it in dataset:
        label = it[-1]
        if label not in label_count:
            label_count[label] = 0
        label_count[label] += 1
    for k in label_count:
        p = float(label_count[k] / num)
        entropy -= p * np.math.log(p, 2)
    print(entropy)
    return entropy
     

if __name__ == "__main__":
    data, label = load_data(sys.argv[1])
    calcu_info_entropy(data)
    print(label)