import numpy as np
import matplotlib.pyplot as plt
import pandas as  pd

data=pd.read_csv("/home/hcooh/projects/float_crystal/datastepcool/2020-11-8/Ep2=0.2Ee2e=4/crystility",header=None,sep='\s+')
data.columns=["T","C"]
print(data)
plt.plot(data["T"],data["C"])
plt.show()