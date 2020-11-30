import matplotlib.pyplot as plt
import numpy as np

def scanCost(nRecords):
    return nRecords/7

with open('./Documents/plotRangeSearchStats.txt') as f:
    l = f.readline()
    while ("TREE" not in l):
        l = f.readline()
    tree_lines = []
    l = f.readline()
    while (l != "\n"):
        tree_lines.append(l)
        l = f.readline()
    x_tree = [int(line.strip().split(" ")[0]) for line in tree_lines]
    y_tree = [int(line.strip().split(" ")[1]) for line in tree_lines]

    while ("HASH" not in l):
        l = f.readline()
    hash_lines = []
    l = f.readline()
    while (l != "\n"):
        hash_lines.append(l)
        l = f.readline()
    x_hash = np.array([int(line.strip().split()[0]) for line in hash_lines])
    y_hash = np.array([int(line.strip().split()[1]) for line in hash_lines])

plt.plot(x_tree, y_tree , label="Tree")
plt.plot(x_hash, y_hash, label="Hash")
plt.plot(x_hash, scanCost(x_hash), 'r--', label="Scan Cost")
# plt.yscale("log")
# plt.xscale("log")
plt.xlim(right=1E6)
plt.xlim(left=0)
plt.ylim(top=1E6)
plt.ylim(bottom=0)
plt.xlabel('Number of Records')
plt.ylabel('Number of Page Reads')
plt.title('Range Search')
plt.legend()
# plt.show()
plt.savefig("./Documents/rangeSearchReadsPlot.png", dpi=300)

plt.clf()

plt.plot(x_tree, np.divide(y_tree, x_tree), label="Tree")
plt.plot(x_hash, np.divide(y_hash, x_tree), label="Hash")
plt.plot(x_hash, np.divide(scanCost(x_hash), x_hash), 'r--', label="Scan Cost")
plt.yscale("log")
plt.xscale("log")
plt.xlim(right=1E6)
# plt.ylim(bottom=0)
plt.ylim(top=1E6)
plt.xlabel('Number of Records (log scale)')
plt.ylabel('Number of Page Reads / Record (log scale)')
plt.title('Range Search')
plt.legend()
# plt.show()
plt.savefig("./Documents/rangeSearchReadsPerRecordLogPlot.png", dpi=300)

plt.clf()

plt.plot(x_tree, y_tree, label="Tree")
plt.plot(x_hash, y_hash, label="Hash")
plt.plot(x_hash, scanCost(x_hash), 'r--', label="Scan Cost")
# plt.yscale("log")
plt.xscale("log")
plt.xlim(right=1E6)
# plt.xlim(left=0)
plt.ylim(bottom=0)
plt.ylim(top=1E6)
plt.xlabel('Number of Records (log scale)')
plt.ylabel('Number of Page Reads')
plt.title('Range Search')
plt.legend()
# plt.show()
plt.savefig("./Documents/rangeSearchReadsLogPlot.png", dpi=300)

plt.clf()

with open('./Documents/plotSearchStats.txt') as f:
    l = f.readline()
    while ("TREE" not in l):
        l = f.readline()
    tree_lines = []
    l = f.readline()
    while (l != "\n"):
        tree_lines.append(l)
        l = f.readline()
    x_tree = [int(line.strip().split(" ")[0]) for line in tree_lines]
    y_tree = [int(line.strip().split(" ")[1]) for line in tree_lines]

    while ("HASH" not in l):
        l = f.readline()
    hash_lines = []
    l = f.readline()
    while (l != "\n"):
        hash_lines.append(l)
        l = f.readline()
    x_hash = np.array([int(line.strip().split()[0]) for line in hash_lines])
    y_hash = np.array([int(line.strip().split()[1]) for line in hash_lines])

plt.plot(x_tree, np.divide(y_tree, x_tree), label="Tree")
plt.plot(x_hash, np.divide(y_hash, x_hash), label="Hash")
# plt.plot(x_hash, scanCost(x_hash), 'r--', label="Scan Cost")
# plt.yscale("log")
# plt.xscale("log")
# plt.xlim(right=1E6) 
# plt.xlim(left=0) 
# plt.ylim(top=1E6) 
# plt.ylim(bottom=0) #
plt.xlabel('Number of Records')
plt.ylabel('Number of Page Reads / Record')
plt.title('Search')
plt.legend()
# plt.show()
plt.savefig("./Documents/searchReadsPlot.png", dpi=300)

    
