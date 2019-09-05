import os
import time

from pyroom import *


class Simulator():
    def __init__(self):
        pass

    def install_model(self, parameter):
        pass

    def simulate(self):
        pass

    def parameters(self):
        pass


class PBS_PBMS(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()
        if not os.path.exists('Complex'):
            os.mkdir('Complex')

        pass

    @staticmethod
    def install_model(r: pyRoom, d):
        for i in range(0, r.shape[1]):
            r.py_input_one_ECC([0, i, 8], 8, 2, [1] * 8, 1)
        for i in range(0, r.shape[2], d):
            r.py_input_one_ECC([15, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)
        for i in range(0, r.shape[2], 3):
            r.py_input_one_ECC([62, 0, i], r.shape[1], 1, [0] * r.shape[1], 1)

        for i in range(1, r.shape[0] - 1):
            if i == 15 or i == 62:
                continue
            for j in range(0, r.shape[1] - 1, 2):
                r.py_input_one_FCC([i, j, 0], 64, 2, 1, [1] * 64, 0)

    def parameters(self):
        import itertools
        Ep12 = list(range(1, 3, 1))
        # length = [32,64,128]
        # T = list(np.arange(2, 6, 0.5))
        length = [32]
        T = [1.5, 2, 2.5, 3]
        return itertools.product(Ep12, length, T)

    def simulate(self, parameter):
        Ep12, length, T = parameter[0], parameter[1], parameter[2]
        try:
            print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep12, 0, T, length, os.getpid()))
            start = time.time()

            r = pyRoom(32, 32, 32, Ep=[[0, 0, 0], [0, 0, Ep12], [0, Ep12, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])

            print("installed model")
            r.movie(200000, 10000, 100)
            # r.draw(title="chain-%3.2f.json" % (T))
            # r.save("Complex/chain-%3.2raw.json" % (T))
            ###########################
            r.movie(1000000, 10000, T)
            # r.draw(title="chain-%3.2f.json" % (T))
            # r.save("Complex/chainEp-0,%3.2f,%3.2f,T=%3.2f.json" % (Ep1, Ep12, T))
            r.save("Complex/chainEp-0,%3.2f,%3.2f,Eb=0,0,%3.2f,T=%3.2f.json" % (0, Ep12, 0, T))

            end = time.time()
            print('Task%f runs %0.2f seconds.' % (T, (end - start)))
        except Exception as e:
            print(e)
            raise e
        return
