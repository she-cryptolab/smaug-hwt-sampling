
# %%
import numpy as np
import seaborn as sb

with open("../build/bin/smaug1-deg-dist.txt", "r") as txt_file:
    meta_line = txt_file.readline()
    data_line = txt_file.readline()

meta = [int(n) for n in meta_line.strip().split()]
data = [int(n) for n in data_line.strip().split()]

hwt = meta[0]; ntests = meta[1]; dimension = meta[2]

average = (hwt*ntests)/dimension
print(" - deg_dist len = " + str(len(data)))
print(" - expected average = ", average)

deg_idx = [int(i) for i in range(len(data))]
sb.displot(data, x=deg_idx)

# %%
# %%
