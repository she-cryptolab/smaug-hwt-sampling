# %%
import numpy as np
import seaborn as sb
import matplotlib.pyplot as plt

path = "./sample/0824-deg-check-i-1/"


def load_data(filepath):
    with open(filepath, "r") as file:
        meta_line = file.readline()
        meta = [int(n) for n in meta_line.strip().split()]
        lines = file.readlines()
    data = [n for n in range(meta[1])]
    matrix = np.array([list(map(int, line.strip().split())) for line in lines])
    for i in range(meta[1]):
        data[i] = matrix[:, i]
    return (meta, data)


def deg_i_dist_plot(data, bin_num, x_label="idx", title="distribution", hist=False):
    if(hist):
        # sb.distplot(data, x= idx, kde = False)
        sb.histplot(data, color='skyblue', alpha=0.7,
                    edgecolor='white', bins=bin_num)
    else:
        plt.scatter(range(len(data)), data, marker='o')
    plt.xlabel(x_label)
    plt.title(title)
    plt.show()


def deg_i_dist_analysis(filepath):
    (meta, data) = load_data(filepath)
    dim = meta[0]
    hwt = meta[1]
    print("dimension =", dim, ", hs = ", hwt)

    for i in range(hwt):
        max = np.max(data[i])
        print("max value = ", max)
        print("max bit = ", np.log2(max))

        # idx = [n for n in range(i)]
        deg_i_dist_plot(data[i], dim-hwt+i, hist=True,
                        title=str(dim-hwt+i)+" dist")


# deg_i_dist_analysis(path+"out1.txt")
# deg_i_dist_analysis(path+"out3.txt")
# deg_i_dist_analysis(path+"out5.txt")

deg_i_dist_analysis(path+"out1-origin.txt")
# deg_i_dist_analysis(path+"out3-origin.txt")
# deg_i_dist_analysis(path+"out5-origin.txt")
# %%
