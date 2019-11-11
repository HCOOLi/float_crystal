import os
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
        length = [128]
        # T = [2.2, 2.4, 2.6, 2.8,3.2, 3.4, 3.6, 3.8,4.0,4.2]
        T = list(np.arange(2.8, 2.9, 0.1))
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
        nums = 0
        for i in range(0, int((r.shape[0] - 2))):
            for j in range(0, r.shape[1] - 2, 2):
                if nums > 1420:
                    return
                typelist = [1 for _ in range(128)]
                r.py_input_one_FCC([i, j, 0], 128, 2, 1, typelist, 0)
                nums = nums + 1

    @staticmethod
    def simulate(parameter):

        try:
            Ep, d, T = parameter[0], parameter[1], parameter[2]
            print('Run task %f ,%f,%f(%s)...' % (Ep, 1, T, os.getpid()))
            date = "2019-11-7-q=27 a=64_64_64c=1.0"
            if not os.path.exists('Data'):
                os.mkdir('Data')
            if not os.path.exists('Data/' + date + '/'):
                os.mkdir('Data/' + date + '/')
            r = pyRoom(64, 64, 64, Ep=[[0, 0, 0], [0, 1, 0], [0, 0, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]],
                       roomtype=24)
            r.q = 27
            SecondNuclear.install_model(r, d)
            r.save('Data/' + date + '/d=%dE%d=%3.2f,T=%3.2f.data' % (d, -1, Ep, T * Ep))
            r.preheat(500000)
            print("end preheat")
            file = open("crystallity.data", "w")
            for i in range(2000):
                r.movie(1280, 10000, T * Ep)
                print("after movie%d" % (i))
                file.write("%d\t%d\n\r" % (i * 10, r.cal_crystallinity(5)))
                file.flush()
            file.close()

            # r.save('Data/' + date + '/d=%dE%d=%3.2f,T=%3.2f.data' % (d, i*10 , Ep, T * Ep))

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
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(1) as p:

            p.map_async(S.simulate, parameter_list)
            p.close()
            p.join()
    except:
        print("shutdown")
        p.terminate()
        p.shutdown(wait=False)
    #
    # print('All subprocesses done.')
    # end = time.time()
    # print('Tasks runs %0.2f seconds.' % (end - start))
