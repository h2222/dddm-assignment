import numpy as np
from scipy.spatial.distance import cdist
import random
import matplotlib.pyplot as plt
import copy


def eclud_distance(vec1, vec2):
    return np.sqrt(np.sum(np.power(vec1 - vec2, 2)))


def total_cost(dataMat, medoids):
    """
    计算总代价
    :param dataMat: 数据对象集
    :param medoids: 中心对象集,是一个字典，
    {0: 0-cluster的索引；1: 1-cluster的索引；k: k-cluster的索引；cen_idx:存放中心对象索引；t_cost:存放总的代价}
    """ 
    med_idx = medoids['cen_idx']
    k = len(med_idx)
    cost = 0
    med_obj = dataMat[med_idx, :]
    dis = cdist(dataMat, med_obj, 'euclidean') # 所有样本到中心点的距离
    cost = dis.min(axis=1).sum() # 判责函数
    medoids['t_cost'] = cost



# 根据促中心, 将其他点根据距离分类进促
def assment(dataMat, medoids):
    med_idx = medoids['cen_idx'] # 促中心idx
    med = dataMat[med_idx] # 促中心对象
    k = len(med_idx) # 促个数
    dis = cdist(dataMat, med, 'euclidean')
    # print(dis)
    idx = dis.argmin(axis=1) # 最小距离对应的idx
    for i in range(k):
        medoids[i] = np.where(idx == i) # 字典, 数据已idx的形式存在在对应的分类里



def PAM(data, k):
    data = np.mat(data)
    N = len(data)
    cur_medoids = {}
    cur_medoids['cen_idx'] = random.sample(set(range(N)), k) # 在所有数据中, 随机选取k个点最为初始促中心
    assment(data, cur_medoids) # 初始聚类
    print(cur_medoids)
    total_cost(data, cur_medoids) # 计算初始促中心的E
    old_medoids = {}
    old_medoids['cen_idx'] = []

    iter_counter = 1
    while set(old_medoids['cen_idx']) != set(cur_medoids['cen_idx']):
        print("iteration count : %d" % iter_counter)
        iter_counter += 1
        best_medoids = copy.deepcopy(cur_medoids)
        old_medoids = copy.deepcopy(cur_medoids)
        for i in range(N): # 对于每一个数据点
            for j in range(k): # 对于每一个聚类
                if i != j: # 对非中心点一次替换为中心点
                    temp_medoids = copy.deepcopy(cur_medoids)
                    temp_medoids['cen_idx'][j] = i #挨个替换促中心idx
                    assment(data, temp_medoids)
                    total_cost(data, temp_medoids)
                    if best_medoids['t_cost'] > temp_medoids['t_cost']:
                        best_medoids = copy.deepcopy(temp_medoids)
        cur_medoids = copy.deepcopy(best_medoids)
        print('current total cost is: %d' % cur_medoids['t_cost'])
    return cur_medoids


def graph(data, medoids):
    fig = plt.figure()
    rect = [0.1, 0.1, 0.8, 0.8]
    ax1 = fig.add_axes(rect, label='ax1', frameon=True)
    ax1.set_title('Clusters Result')
    
    ax1.scatter(data[medoids[0], 0], data[medoids[0], 1], c='r')
    ax1.scatter(data[medoids[1], 0], data[medoids[1], 1], c='g')
    ax1.scatter(data[medoids[2], 0], data[medoids[2], 1], c='y')
    ax1.scatter(data[medoids['cen_idx'], 0], data[medoids['cen_idx'], 1], marker='x', s=500)
    plt.show()


def run_pam(k=3, dim=2, N=100):
    # create data with different normal distribution
    d1 = np.random.normal(1, .2, (N, dim))
    d2 = np.random.normal(2, .5, (N, dim))
    d3 = np.random.normal(3, .5, (N, dim))

    data = np.vstack((d1, d2, d3))
    medoids = PAM(data, k)
    graph(data, medoids)



if __name__ == "__main__":
    run_pam()