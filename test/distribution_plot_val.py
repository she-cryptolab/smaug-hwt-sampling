
# %%
# --------------- degree distribution --------------- #
# - after duplicate prevention
# - input is the set of degree of the each coeff

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sb

NSAMPLE = 1
path = "./sample/0830-deg-check-1/"


def deg_load_data(filepath):
    with open(filepath, "r") as txt_file:
        meta_line = txt_file.readline()
        meta = [int(n) for n in meta_line.strip().split()]
        data_line = txt_file.readline()
        data = [int(n) for n in data_line.strip().split()]
    return meta, data


def deg_dist_plot(data, x_label="idx", title="distribution", hist=False):
    if(hist):
        # sb.distplot(data, x= idx, kde = False)
        sb.histplot(data, color='skyblue', alpha=0.7,
                    edgecolor='white')
    else:
        plt.scatter(range(len(data)), data, marker='o')
    plt.xlabel(x_label)
    plt.title(title)
    plt.show()


def deg_dist_analysis(filepath):
    (meta, data) = deg_load_data(filepath)
    max = np.max(data)

    print("max value = ", max)
    print("max bit = ", np.log2(max))

    deg_dist_plot(data, hist=True)


# deg_dist_analysis(path+"out-bike1-deg-origin.txt")
# deg_dist_analysis(path+"out-bike3-deg-origin.txt")
# deg_dist_analysis(path+"out-bike5-deg-origin.txt")

deg_dist_analysis(path+"smaug1-deg-origin.txt")
deg_dist_analysis(path+"smaug3-deg-origin.txt")
deg_dist_analysis(path+"smaug5-deg-origin.txt")

deg_dist_analysis(path+"smaug1-deg-update.txt")
deg_dist_analysis(path+"smaug3-deg-update.txt")
deg_dist_analysis(path+"smaug5-deg-update.txt")


# %%
