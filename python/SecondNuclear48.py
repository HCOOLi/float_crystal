import os
import random
import time
from multiprocessing import Pool

import numpy as np
from pyroom import *


class Simulator():
    def __init__(self):
        pass

    def install_model(self, *parameter):
        pass

    def simulate(self, *key):
        pass

    def parameters(self):
        pass


class SecondNuclear(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()
        if not os.path.exists('Complex'):
            os.mkdir('Complex')
        pass

    def parameters(self):
        import itertools
        Ep = [1.0]
        length = [64]
        # T = [2.2, 2.4, 2.6, 2.8,3.2, 3.4, 3.6, 3.8,4.0,4.2]
        T = list(np.arange(2.0, 2.30, 0.02))
        d = [0]
        return itertools.product(Ep, d, T)

    @staticmethod
    def install_model(r, d):
        """
        :param r:
        :type r: pyRoom
        :param d:
        :return:
        """
        for i in range(3, r.shape[1] - 3):
            r.py_input_one_ECC([0, i, 8], 20, 2, [1] * 20, 1)
        for i in range(8, 29):
            r.py_input_one_ECC([63, 3, i], r.shape[1] - 6, 1, [0] * (r.shape[1] - 6), 1)
        # for i in range(0, r.shape[2], d):
        #     r.py_input_one_ECC([15, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
        # for i in range(0, r.shape[2], 3):
        #     r.py_input_one_ECC([62, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
        nums = 0
        for i in range(3, int((r.shape[0] - 2))):
            # if i == 15 or i == 62:
            #     continue

            for j in range(0, r.shape[1] - 2, 2):
                nums = nums + 1

                typelist = [(1 if random.random() < 1.1 else 2) for _ in range(64)]
                r.py_input_one_FCC([i, j, 0], 64, 2, 1, typelist, 0)

    @staticmethod
    def simulate(parameter):

        try:
            Ep, d, T = parameter[0], parameter[1], parameter[2]
            print('Run task %f ,%f,%f(%s)...' % (Ep, 1, T, os.getpid()))
            date = "2019-10-4-q=135a=64_32_40c=1.0"
            if not os.path.exists('Data'):
                os.mkdir('Data')
            if not os.path.exists('Data/' + date + '/'):
                os.mkdir('Data/' + date + '/')
            r = pyRoom(64, 32, 40, Ep=[[0, 0, 0], [0, Ep, 0], [0, 0, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]],
                       roomtype=24)
            r.q = 135
            SecondNuclear.install_model(r, d)
            r.save('Data/' + date + '/d=%dE%d=%3.2f,T=%3.2f.data' % (d, -1, Ep, T * Ep))
            r.preheat(5000000)
            print("end preheat")
            for i in range(2000):
                r.movie(20000, 5000, T * Ep)
                r.save('Data/' + date + '/d=%dE%d=%3.2f,T=%3.2f.data' % (d, i * 20000, Ep, T * Ep))

        except Exception as e:
            print(e)
            raise e

        return


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    S = SecondNuclear()
    parameter_list = list(S.parameters())
    # S.simulate(parameter_list[1])
    # exit(0)
    try:
        with Pool(11) as p:

            p.map_async(S.simulate, parameter_list)
            p.close()
            p.join()
    except:
        print("shutdown")
        p.terminate()
        p.shutdown(wait=False)
    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
