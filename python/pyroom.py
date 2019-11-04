import json
from typing import List

import matplotlib.pyplot as plt
import numpy as np
from float_crystal import *


class pyRoom(pyroom):
    def __init__(self, a, b, c, Ep, Eb, roomtype):
        pyroom.__init__(self, int(a), int(b), int(c), Ep, Eb, int(roomtype))
        self.shape = np.asarray([a, b, c])


    def py_input_one_ECC(self, a, length, direction, ty, movable):
        # type: (List[int], int, int, List, int) -> None

        self.input_one_ECC(int(a[0]), int(a[1]), int(a[2]), int(length), int(direction), ty, int(movable))

    def py_input_one_FCC(self, a, length, direction, fold_direction, ty, movable):
        # type: (List[int], int, int, int, List, int) -> None
        self.input_one_FCC(int(a[0]), int(a[1]), int(a[2]), int(length), int(direction),
                           int(fold_direction), ty, int(movable))

    # def save(self, file_path):
    #     parameters = {"Ep": self.Ep, "Eb": self.Eb}
    #     saving_dict = {"parameters": parameters, "data": self.get_list()}
    #     with open(file_path, 'w') as file:
    #         # file.write(json.dumps(self.get_list()))
    #         file.write(json.dumps(saving_dict))

    def new_draw_box(self, point1, point2, box_color='blue'):
        from vpython import vector, curve, color
        # def into_vector(a):
        #     return vector(a[0], a[1], a[2])
        # print("draw a box")
        p1 = [0, 0, 0]
        p2 = [0, 0, 0]
        radius = 0.04
        if box_color == 'blue':
            box_color = color.black
        if box_color == 'red':
            box_color = color.red
        for a in range(2):
            for b in range(2):
                for c in range(3):
                    p1[c], p2[c] = point1[c], point2[c]

                    p1[(c + 1) % 3] = point1[(c + 1) % 3] if a == 0 else point2[(c + 1) % 3]
                    p2[(c + 1) % 3] = point1[(c + 1) % 3] if a == 0 else point2[(c + 1) % 3]
                    p1[(c + 2) % 3] = point1[(c + 2) % 3] if b == 0 else point2[(c + 2) % 3]
                    p2[(c + 2) % 3] = point1[(c + 2) % 3] if b == 0 else point2[(c + 2) % 3]
                    c = curve(vector(p1[0], p1[1], p1[2]), vector(p2[0], p2[1], p2[2]), color=box_color, radius=radius)

    def draw_box(self):
        self.new_draw_box([0, 0, 0], self.shape)

    def draw_all(self, polylist=None, title=None):
        from vpython import canvas, vector, curve, color

        scene = canvas(title=title, width=800, height=800,
                       center=vector(self.shape[0] / 2, self.shape[1] / 2, self.shape[2] / 2), background=color.white)
        self.draw_box()
        nums = self.get_num_of_polymers()
        for i in range(nums):
            chain = self.get_polymer(i)

            # print(chaintype)

            # for chain in polylist:

            c = curve(color=color.yellow, radius=0.1)
            if chain:
                point2 = chain.get_list()["chain"][0].copy()['position']
                type = chain.get_list()["chain"][0].copy()["type"]
                if type == 1:
                    this_color = color.yellow
                elif type == 2:
                    # continue
                    this_color = color.blue
                elif type == 3:
                    continue
                    this_color = color.red
                c = curve(color=this_color, radius=0.1)
            else:
                continue

            for pointinfo in chain.get_list()["chain"]:
                point = pointinfo['position']
                type = pointinfo["type"]
                if type == 1:
                    this_color = color.yellow
                elif type == 2:
                    # continue
                    this_color = color.blue
                elif type == 3:
                    continue
                    this_color = color.red
                if (self.if_out_of_range(point2, point)):
                    pass
                else:
                    c = curve(color=this_color, radius=0.1)

                c.append(vector(point[0], point[1], point[2]))
                # sphere(pos=vector(point[0], point[1], point[2]), color=this_color, radius=0.2)
                point2 = point.copy()
        return scene

    def draw_a_layer(self, layer, polylist=None, title=None):
        from vpython import canvas, vector, curve, color, sphere

        scene = canvas(title=title, width=800, height=800,
                       center=vector(self.shape[0] / 2, self.shape[1] / 2, self.shape[2] / 2), background=color.white)
        self.draw_box()
        nums = self.get_num_of_polymers()
        for i in range(nums):
            chain = self.get_polymer(i)

            c = curve(color=color.yellow, radius=0.1)
            if chain:
                point2 = chain.get_list()["chain"][0].copy()['position']

                type = chain.get_list()["chain"][0].copy()["type"]
                if type == 1:
                    this_color = color.yellow
                elif type == 2:
                    # continue
                    this_color = color.blue
                elif type == 3:
                    continue
                    this_color = color.red

                c = curve(color=this_color, radius=0.1)
            else:
                continue

            for pointinfo in chain.get_list()["chain"]:
                point = pointinfo['position']
                type = pointinfo["type"]
                if type == 1:
                    this_color = color.yellow
                elif type == 2:
                    # continue
                    this_color = color.blue
                elif type == 3:
                    continue
                    this_color = color.red
                if point[0] != layer:
                    continue
                else:
                    sphere(pos=vector(point[0], point[1], point[2]), color=this_color, radius=0.2)
                    if (self.if_out_of_range(point2, point)):
                        pass
                    else:
                        c = curve(color=this_color, radius=0.1)

                    c.append(vector(point[0], point[1], point[2]))

                    point2 = point.copy()
        return scene

    def draw_a_layer_plot(self, layer, polylist=None, title=None):

        nums = self.get_num_of_polymers()
        plt.figure(figsize=(10, 5))
        ax1 = plt.subplot(1, 2, 1)
        ax1.plot([0, 32], [8, 8])
        ax1.plot([0, 32], [23, 23])
        ax2 = plt.subplot(1, 2, 2)
        countlist = []
        count = 0
        for i in range(nums):
            chain = self.get_polymer(i)

            # c = curve(color=color.yellow, radius=0.1)
            last_point = None
            if count != 0:
                countlist.append(count)
                count = 0
            if chain:
                last_point = chain.get_list()["chain"][0].copy()['position']
                type = chain.get_list()["chain"][0].copy()["type"]
                if type == 1:
                    pass
                    # this_color = color.yellow
                elif type == 2:
                    pass
                    # continue
                    # this_color = color.blue
                elif type == 3:
                    continue
                    # this_color = color.red

                # c = curve(color=this_color, radius=0.1)
                last_point = None
                if count != 0:
                    countlist.append(count)
                    count = 0
            else:
                continue

            for pointinfo in chain.get_list()["chain"]:
                point = pointinfo['position']
                type = pointinfo["type"]
                if type == 1:
                    pass
                    # this_color = color.yellow
                elif type == 2:
                    pass
                    # continue
                    # this_color = color.blue
                elif type == 3:
                    continue
                if point[0] != layer:
                    continue
                else:
                    # sphere(pos=vector(point[0], point[1], point[2]), color=this_color, radius=0.2)
                    if last_point != None:
                        if (self.if_out_of_range(last_point, point)):
                            pass
                        else:
                            last_point = None
                    if last_point != None:
                        if point[1] == last_point[1] and 8 <= min(point[2], last_point[2]) and max(point[2],
                                                                                                   last_point[2]) < 24:
                            if this_color == 'grey':
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
                                count += 1
                            else:
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="red")
                                count += 1
                        else:
                            if this_color == 'grey':
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
                            else:
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="blue")
                            if count != 0:
                                countlist.append(count)
                                count = 0
                    last_point = point.copy()

            point2 = None
            if count != 0:
                countlist.append(count)
                count = 0
        plt.figure()
        histlist, bins, _ = plt.hist(countlist, bins=range(1, 17, 1))
        # print(histlist, bins)
        plt.close()
        # countlist

        ax2.bar(bins[:-1], np.asarray(histlist) * np.asarray(bins[:-1]))
        plt.ylim(0, 80)
        plt.xlim(0, 16)

        # plt.show()

        # return scene

    def draw_a_layer_plot_json(self, layer, polylist, title=None):
        plt.figure(figsize=(10, 5))
        ax1 = plt.subplot(1, 2, 1)
        ax1.plot([0, 32], [8, 8])
        ax1.plot([0, 32], [29, 29])
        ax2 = plt.subplot(1, 2, 2)
        countlist = []
        count = 0
        for chain in polylist:

            # c = curve(color=color.yellow, radius=0.1)
            last_point = None
            if count != 0:
                countlist.append(count)
                count = 0
            if chain:
                last_point = chain["c"][0].copy()['p']
                type = chain["c"][0].copy()["t"]
                if type == 1:

                    this_color = 'any'
                elif type == 2:
                    this_color = 'grey'


                elif type == 3:
                    continue
                    # this_color = color.red

                # c = curve(color=this_color, radius=0.1)
                last_point = None
                if count != 0:
                    countlist.append(count)
                    count = 0
            else:
                continue

            for pointinfo in chain["c"]:
                point = pointinfo['p']
                type = pointinfo["t"]
                if type == 1:
                    this_color = 'any'
                elif type == 2:
                    # pass

                    this_color = 'grey'
                elif type == 3:
                    continue
                if point[0] != layer:
                    continue
                else:
                    # sphere(pos=vector(point[0], point[1], point[2]), color=this_color, radius=0.2)
                    if last_point != None:
                        if (self.if_out_of_range(last_point, point)):
                            pass
                        else:
                            last_point = None
                    if last_point != None:
                        if point[1] == last_point[1] and 8 <= min(point[2], last_point[2]) and max(point[2],
                                                                                                   last_point[2]) < 29:
                            if this_color == 'grey':
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
                                count += 1
                            else:
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="red")
                                count += 1
                        else:
                            if this_color == 'grey':
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="grey")
                            else:
                                ax1.plot([point[1], last_point[1]], [point[2], last_point[2]], color="blue")
                    last_point = point.copy()

            point2 = None
            if count != 0:
                countlist.append(count)
                count = 0
        plt.figure()
        histlist, bins, _ = plt.hist(countlist, bins=range(1, 21, 1))
        # print(histlist, bins)
        plt.close()
        # countlist

        ax2.bar(bins[:-1], np.asarray(histlist) * np.asarray(bins[:-1]))
        plt.ylim(0, 80)
        plt.xlim(0, 21)

        return countlist

        # return scene

    def load_polymer(self, filepath):
        T = 0
        time = 1
        with open(filepath, 'r') as file:
            all_line_txt = file.readline()  # 读所有行
            # all_line_txt = all_line_txt.replace("position", "p").replace("type", "t").replace("chain", "c").replace(
            #     "moveable", "m")
            polymerlist = json.loads(all_line_txt)
        # with open(filepath, 'w') as file:
        #     file.writelines(all_line_txt)

        try:
            return polymerlist['data']
        except:
            return polymerlist

    def step_heating(self, start, end, step, time_length, time_step, EC_max):
        E_list, Ec_list, Ep_list, t_list = [], [], [], []
        print(start, end, step)
        for i in np.arange(start, end, step):
            self.movie(time_length, time_step, i)
            E, Ec, Ep, Eb = self.get_result()
            E_list += E
            Ec_list += Ec
            Ep_list += Ep
            t_list += [i] * int(time_length / time_step)
            # self.save(i, i*1000)
        f = []
        for i in Ec_list:
            f.append(-i / EC_max)
        return E_list, Ec_list, Ep_list, t_list, f

    def if_out_of_range(self, point2, point1):
        for i in range(3):
            if (abs(point2[i] - point1[i]) > 1):
                return False

        return True

    def cal_crystal(self):

        r0 = pyRoom(self.shape[0], self.shape[2], self.shape[1], Ep=self.Ep, Eb=self.Eb)
        num_of_chains = self.shape[0] * self.shape[1] / 4
        chain_length = self.shape[2] / 4 * 3
        EC_max = num_of_chains * (chain_length - 1)
        r0.py_inputECC(num_of_chains, chain_length)
        Ep0 = r0.cal_Ep()

        thicka, thickb, thickc = self.cal_thick_by_point()
        print('Ep=%f,Eb=%f,length=%f   a' % (self.Ep, self.Eb, self.shape[2]))
        print("Ep结晶度：%3.1f%%" % (self.cal_Ep() / Ep0 * 100))
        print("Ec结晶度：%3.1f%%" % ((self.cal_Ec() / EC_max + 1) * 100))
        print("max=%d" % max(thickc))
        plt.title('Ep=%f,Eb=%f,length=%f   a' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thicka)
        plt.show()
        plt.title('Ep=%f,Eb=%f,length=%f   b' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thickb)
        plt.show()
        plt.title('Ep=%f,Eb=%f,length=%f   c' % (self.Ep, self.Eb, self.shape[2]))
        plt.hist(thickc)
        plt.show()

    def py_cal_thickness(self):
        thicknesslist = self.cal_thickness()
        print(thicknesslist)
        plot_list = []
        sort_list = []
        for lammellar in thicknesslist:
            if lammellar:
                a = lammellar[0] - lammellar[3]
                b = lammellar[1] - lammellar[4]
                c = lammellar[2] - lammellar[5]
                self.new_draw_box([lammellar[0], lammellar[1], lammellar[2]],
                                  [lammellar[3], lammellar[4], lammellar[5]],
                                  'red')
                plot_list.append([b, c])
                sort_list.append(c)
        sort_list.sort(reverse=True)

        plt.figure()
        plt.title('Ep=%f,Eb=%f,length=%f  ' % (self.Ep, self.Eb, self.shape[2]))
        plt.xlabel('b')
        plt.ylabel('c')
        plt.scatter(x=np.asarray(plot_list)[:, 0], y=np.asarray(plot_list)[:, 1])
        plt.show()
        return np.mean(np.asarray(sort_list[:5]))
