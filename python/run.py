def reconstruct(parameter):
    Ep, Eb, T, length, T_anneal, steps = parameter["Ep"], parameter["Eb"], parameter["T"], \
                                         parameter["length"], parameter["T_anneal"], parameter["steps"]
    k = length * 3 / 4 - 4
    if T_anneal != 0:
        loadpath = "steps%d/chain%d/chain-%d,%d,%d,%d-annealed in%d.json" % \
                   (steps, length, Ep * 10, Eb * 10, T * 10, k, T_anneal * 10)
        # loadpath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d-annealed in%3.2f.json" % \
        #            (length, Ep, Eb, T, k, T_anneal)
    else:
        # loadpath = "steps%d/chain%d/chain-%d,%d,%d,%d.json" % (steps,length, Ep * 10, Eb * 10, T * 10, k)
        # loadpath = "k=4/steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k)
        # loadpath = "k=4/steps%d/chain%d/chain-%3.2f,%3.2f,%3.2fend.json" % (steps, length, Ep, Eb, T)
        loadpath = "Complex/chain-%3.2f.json" % (T)

    try:
        print('Run task steps=%d Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (steps, Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, 128, Ep=[[0, 0, 0], [0, 1, 2], [0, 2, 1]], Eb=[[0, 0, 0], [0, 0, 0], [0, 0, 0]])
        r.construct_by_pylist(r.load_polymer(filepath=loadpath))
        # r.draw(path=loadpath)
        r.draw(title=loadpath)
        print(r.py_cal_thickness())

        # r.cal_crystal()

        os.system("pause")
    except Exception as e:
        print(e)
        print("subprocess wrong")
        raise Exception("subprocess error ")
    # return


def anneal(parameter):
    Ep, Eb, T, length, T_anneal = parameter["Ep"], parameter["Eb"], parameter["T"], \
                                  parameter["length"], parameter["T_anneal"]

    k = length * 3 / 4 - 4
    loadpath = "chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k)
    # loadpath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (length, Ep, Eb, T, k)
    savepath = "chain%d/chain-%3.2f,%3.2f,%3.2f,%d-annealed in%3.2f.json" % \
               (length, Ep, Eb, T, k, T_anneal)
    try:
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        r = pyRoom(32, 32, length, Ep=Ep, Eb=Eb)
        r.construct_by_pylist(r.load_polymer(loadpath))

        r.movie(30000 * int(length / 12), 20000, T_anneal)
        r.save(savepath)
        # r.draw(path="chain%d/chain-%d,%d,%d,%d.json" % (length, Ep * 10, Eb * 10, T * 10, k))
        # r.cal_crystal()
    except Exception as e:
        print(e)
        print("subprocess wrong")
        raise Exception("subprocess error ")
    # return


def washing_small(parameter):
    try:
        Ep, Eb, T, length, steps = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"], parameter[
            "steps"]
        print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
        start = time.time()

        r = pyRoom(24, 24, length, Ep=Ep, Eb=Eb)
        r.py_inputECC_with_small()
        if not os.path.exists('steps%d' % steps):
            os.mkdir('steps%d' % steps)
        if not os.path.exists('steps%d/chain%d' % (steps, length)):
            os.mkdir('steps%d/chain%d' % (steps, length))
        k = 0
        for i, _ in enumerate(r.remove_c_layer()):
            if (i % 4) == 0:
                k = i * 2
                r.movie(steps, 20000, T)

                if k % 24 == 0:
                    print("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k))
                    r.save("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2f,%d.json" % (steps, length, Ep, Eb, T, k))
                pass

        r.save("steps%d/chain%d/chain-%3.2f,%3.2f,%3.2fend.json" % (steps, length, Ep, Eb, T))
        end = time.time()
        print('Task%fruns %0.2f seconds.' % (Ep, (end - start)))
    except Exception as e:
        print(e)
        raise Exception("subprocess error")

    return


def washing_small_a_b(parameter):
    Ep, Eb, T, length = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"]

    # try:
    print('Run task Ep=%f ,Eb=%f,T=%f,length=%d(%s)...' % (Ep, Eb, T, length, os.getpid()))
    start = time.time()

    r = pyRoom(32, 32, 128, Ep=Ep, Eb=Eb)
    EC_max = 16 * 16 * (64 - 1)
    r.py_inputECC_with_small()

    for k in range(0, int(r.shape[0]), 2):
        r.remove_a_layer(k)
        r.remove_a_layer(k + 1)
        r.remove_b_layer(k)
        r.remove_b_layer(k + 1)
        r.remove_c_layer()
        # r.remove_c_layer(k + 2)
        #     # r.remove_c_layer(k + 4)
        #     # r.remove_c_layer(k + 6)
        #     # r.remove_c_layer(k + 8)
        #     # r.remove_c_layer(k + 10)
        #     # r.remove_c_layer(k + 12)
        r.movie(20000, 20000, T)
        r.save("chainabc/chain-%d,%d,%d,%d.json" % (Ep * 10, Eb * 10, T * 10, k))

    end = time.time()
    print('Task%f ,%fruns %0.2f seconds.' % (Ep, (end - start)))

    return


def step_heating(parameter):
    try:
        Ep, Eb, T, length = parameter["Ep"], parameter["Eb"], parameter["T"], parameter["length"]
        print('Run task %f ,%f,%f(%s)...' % (Ep, Eb, T, os.getpid()))
        start = time.time()
        EC_max = 16 * 16 * (96 - 1)

        r = pyRoom(32, 32, length, Ep=Ep, Eb=Eb)
        r.py_inputECC_with_small()
        for _ in r.remove_c_layer():
            pass
        print("removed all")
        E_list, Ec_list, Ep_list, t_list, f = r.step_heating(6 * Ep, 1 * Ep, -0.1 * Ep, EC_max)
        plt.plot(t_list, f)
        plt.savefig("stepheating%3.2f,%3.2f.png" % (Ep, Eb))
        plt.show()

    except Exception as e:
        print(e)
        raise e

    return
