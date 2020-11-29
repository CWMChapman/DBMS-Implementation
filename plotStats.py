import matplotlib.pyplot as plt



with open('plotStats.txt') as f:
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
    x_hash = [int(line.strip().split()[0]) for line in hash_lines]
    y_hash = [int(line.strip().split()[1]) for line in hash_lines]

plt.plot(x_tree, y_tree, label="Tree")
plt.plot(x_hash, y_hash, label="Hash")
plt.yscale("log")
plt.xscale("log")
plt.xlabel('Number of Records')
plt.ylabel('Number of Reads')
plt.title('Range Search')
plt.legend()
plt.show()
plt.save("rangeSearchReadsPlot.png")

    