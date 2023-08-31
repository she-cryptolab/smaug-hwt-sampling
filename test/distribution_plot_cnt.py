
# %%
# --------------- total degree distribution --------------- #
# - after duplicate prevention
# - input is the set of count of the each coeff (degree)

import numpy as np
import seaborn as sb
import matplotlib.pyplot as plt

NSAMPLE = 20
# path = "../build/bin/"
# path = "./sample/0818-sample-cnt-1/"
path = "./sample/0831-sample-cnt-2/"

def load_data(filepath):
    data = [0 for i in range(NSAMPLE)]
    with open(filepath, "r") as txt_file:
        meta_line = txt_file.readline()
        meta = [int(n) for n in meta_line.strip().split()]
        for i in range(NSAMPLE):
            data_line = txt_file.readline()
            data[i] = [int(n) for n in data_line.strip().split()]
    return meta, data


def dist_plot(data, idx, bin_num, x_label="idx", title="distribution", hist=False):
    if(hist):
        # sb.distplot(data, x= idx, kde = False)
        sb.histplot(data, x=idx, color='skyblue', alpha=0.7,
                    edgecolor='white', bins=bin_num)
    else:
        plt.scatter(range(len(data)), data, marker='o')
    plt.xlabel(x_label)
    plt.title(title)
    plt.show()


def deg_dist_analysis(filepath, cnt=False):
    data = [0 for i in range(NSAMPLE)]
    (meta, data) = load_data(filepath)

    hwt = meta[0]
    ntests = meta[1]
    dimension = meta[2]
    average = (hwt*ntests)/dimension

    deg_dist = [0 for i in range(len(data[0]))]
    deg_idx = [int(i) for i in range(len(data[0]))]
    for i in range(NSAMPLE):
        deg_dist = np.array(deg_dist) + np.array(data[i])
    deg_dist = deg_dist/NSAMPLE

    if(cnt):
        dist_plot(deg_dist, deg_idx, dimension,
                  "poly section (rank)", "cnt distribution")
    else:
        dist_plot(deg_dist, deg_idx, dimension,
                  "degree", "degree distribution", hist=True)

 # SMAUG degree distribution
deg_dist_analysis(path+"smaug1-deg-dist-update.txt")
deg_dist_analysis(path+"smaug1-deg-dist-origin.txt")

deg_dist_analysis(path+"smaug3-deg-dist-update.txt")
deg_dist_analysis(path+"smaug3-deg-dist-origin.txt")

deg_dist_analysis(path+"smaug5-deg-dist-update.txt")
deg_dist_analysis(path+"smaug5-deg-dist-origin.txt")

# SMAUG count array distribution
# deg_dist_analysis(path+"smaug1-deg-cnt-update.txt", cnt=True)
# deg_dist_analysis(path+"smaug1-deg-cnt-origin.txt", cnt=True)

# deg_dist_analysis(path+"smaug3-deg-cnt-update.txt", cnt=True)
# deg_dist_analysis(path+"smaug3-deg-cnt-origin.txt", cnt=True)

# deg_dist_analysis(path+"smaug5-deg-cnt-update.txt", cnt=True)
# deg_dist_analysis(path+"smaug5-deg-cnt-origin.txt", cnt=True)


# %%
# --------------- rand distribution --------------- #


NSAMPLE = 1
# path = "../build/bin/"
path = "./sample/0822-rand-1/"
# path = "./sample/0823-buf-1/"


def rand_load_data(filepath):
    with open(filepath, "r") as txt_file:
        meta_line = txt_file.readline()
        meta_line = txt_file.readline()
        meta = [int(n) for n in meta_line.strip().split()]
        data_line = txt_file.readline()
        data = [int(n) for n in data_line.strip().split()]
    return meta, data


def rand_dist_plot(data, bin_num, x_label="idx", title="distribution", hist=False):
    if(hist):
        # sb.distplot(data, x= idx, kde = False)
        sb.histplot(data, color='skyblue', alpha=0.7,
                    edgecolor='white', bins=bin_num)
    else:
        plt.scatter(range(len(data)), data, marker='o')
    plt.xlabel(x_label)
    plt.title(title)
    plt.show()


def rand_dist_analysis(filepath):
    (meta, data) = rand_load_data(filepath)
    max = np.max(data)

    print("max value = ", max)
    print("max bit = ", np.log2(max))

    rand_dist_plot(data, 400, hist=True)


rand_dist_analysis(path+"out1.txt")
rand_dist_analysis(path+"out3.txt")
rand_dist_analysis(path+"out5.txt")

# %%
