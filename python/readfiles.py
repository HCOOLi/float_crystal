import os
import time
from multiprocessing import Pool

import matplotlib.pyplot as plt
import numpy as np
from pyroom import *


def drawpicture(date, T):
    path = './Data/' + date + '/'
    picpath = './Data/' + date + '-1.0-' + T + '/'
    thinpath = './Data/' + date + '/'
    if not os.path.exists(picpath):
        os.mkdir(picpath)
    max = []
    r = pyRoom(64, 32, 40, Ep=[[0, 0, 0], [0, 1, 0], [0, 0, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]], roomtype=24)
    for i in range(0, 10000):
        filename = ('d=0E%d=1.00,T=' + T + '.data') % (i * 100)
        # print("init")
        # r = pyRoom(64, 64, 64, Ep=[[0, 0, 0], [0, 2,0 ], [0, 0, 0]],
        # Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]], roomtype=24)
        print(thinpath + filename)
        print("loading")
        r.load(thinpath + filename)
        print("getlist")
        # Ec.append(r.cal_Ec())
        max.append(r.get_max_nucleus(1))

    mmmax = 0
    mmmax_list = []
    for m in max:
        if m > mmmax:
            mmmax = m
        mmmax_list.append(mmmax)

    plt.plot(mmmax_list, list(range(10000)))
    plt.title(T)
    plt.savefig(picpath + "step_max")
    plt.close()
    plt.plot(max)
    plt.title(T)
    plt.savefig(picpath + "max")
    plt.close()
    # # plt.plot(Ep2)
    # # plt.title(T)
    # # plt.savefig(picpath + "lines2")
    # # plt.plot(Ep3)
    # # plt.title(T)
    # # plt.savefig(picpath + "lines3")
    # plt.close()


        # coutlist = r.draw_a_layer_plot_json(2, polymerlist)
        # plt.savefig(picpath + str(i) + "-2")
        # plt.close()
        # coutlist = r.draw_a_layer_plot_json(3, polymerlist)
        # plt.savefig(picpath + str(i) + "-3")
        # plt.close()

        # plt.show()
        # plt.savefig(picpath+str(i)+"3")
        # plt.close()
        # plt.ylim(0,40)
        # plt.hist(coutlist,bins=list(range(16)),)
        # plt.savefig(picpath + str(i)+"hist2")
        # plt.close()

        # os.system("pause")

# Ecmax = 64 * 31 * (31 - 1)
# with open("Data/Ec_list,Ep2=1.00,T=4.00.json", 'r') as f:
#     for line in f.readlines():
#         Ec_list = json.loads(line)
#         Ec_list = list(map(lambda x: 1 + x / Ecmax, Ec_list))
#         plt.title("Ep2=1.00,T=4.00")
#         plt.ylabel("f")
#         plt.xlabel("100000MC cycle")
#         plt.plot(Ec_list)
#         plt.show()
#         print(Ec_list)
if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    date = "2019-10-4-q=108a=64_32_40c=1.0t=4"
    # S.simulate(parameter_list[1])
    with Pool(12) as p:

        # for T in ["4.60", "4.80", "4.40"]:"2.20","2.40","2.60","2.80",
        # for T in ["3.00","3.20","3.40","3.60","3.80","4.00","4.20","4.60", "4.80", "4.40"]:
        for T in ['%3.2f' % x for x in np.arange(0.1, 0.8, 0.1)]:
            p.apply_async(drawpicture, (date, T))
        p.close()
        p.join()

    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
