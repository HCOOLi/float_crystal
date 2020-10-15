import numpy as np
import os
import time
from multiprocessing import Pool

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


class Inclusion_Complex(Simulator):
    def __init__(self):
        super(Simulator, self).__init__()
        if not os.path.exists('Complex'):
            os.mkdir('Complex')
        pass

    def py_inputECC_with_small(self, r: pyRoom):
        num = 0
        for i in range(r.shape[0]):
            for j in range(r.shape[1]):
                if j % 2 == 0 or i % 2 == 0 or (i + j) % 8 == 0 or (i - j) % 8 == 0:
                    for k in range(0, int(3 * r.shape[2] / 4), 2):
                        r.py_input_one_ECC([i, j, k + int(r.shape[2] / 8)], 2, 2, [2] * 2, 0)

                        num += 1
                else:
                    r.py_input_one_ECC([i, j, 1], r.shape[2] - 5, 2, [1] * (r.shape[2] - 5), 0)
                    num += 1

                pass

    def parameters(self):
        import itertools
        Ep12 = list(np.arange(0.4, 1, 0.8))
        # length = [32,64,128]
        # T = list(np.arange(2, 6, 0.5))
        length = [32]
        T = [1.5]
        return itertools.product(Ep12, length, T)

    def simulate(self, parameter):
        Ep12, length, T = parameter[0], parameter[1], parameter[2]
        try:
            print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep12, 0, T, length, os.getpid()))
            start = time.time()

            r = pyRoom(32, 32, 32, Ep=[[0, 0, 0], [0, 0, Ep12], [0, Ep12, 0]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0.1]])

            self.py_inputECC_with_small(r)

            print("installed model")
            r.movie(20000, 10000, 100)
            # r.draw(title="chain-%3.2f.json" % (T))
            # r.save("Complex/chain-%3.2raw.json" % (T))
            ###########################
            for i in range(4):
                r.movie(30000, 10000, T * Ep12)
                print("after movie%d" % (i))
                r.save("Complex/rand%dchain-%dEp-0,%3.2f,%3.2f,Eb=%3.2f,%3.2f,0,T=%3.2f.json" % (
                    np.random.randint(1, 50), i, 0, Ep12, 0, 0.1, T))

            end = time.time()
            print('Task%f runs %0.2f seconds.' % (T, (end - start)))
        except Exception as e:
            print(e)
            raise e
        return


if __name__ == '__main__':
    start = time.time()
    print('Parent process %s.' % os.getpid())
    S = Inclusion_Complex()
    parameter_list = list(S.parameters())
    print(parameter_list)
    # S.simulate(parameter_list[1])
    try:
        # with ProcessPoolExecutor(max_workers=5) as p:
        with Pool(10) as p:
            for _ in range(10):
                p.map_async(S.simulate, parameter_list)
                time.sleep(1)
            p.close()
            p.join()
    except:
        print("shutdown")
        p.terminate()
        p.shutdown(wait=False)

    print('All subprocesses done.')
    end = time.time()
    print('Tasks runs %0.2f seconds.' % (end - start))
