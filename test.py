coords = []
with open('./dataset/kz9976.tsp') as f:
    start = False
    for line in f:
        if line.strip() == 'NODE_COORD_SECTION':
            start = True
            continue
        if start:
            if line.strip() == 'EOF':
                break
            parts = line.split()
            if len(parts) == 3:
                _, x, y = parts
                coords.append((float(x), float(y)))

import numpy as np

n = len(coords)
dist_matrix = np.zeros((n, n))
for i in range(n):
    for j in range(n):
        if i != j:
            xi, yi = coords[i]
            xj, yj = coords[j]
            dist_matrix[i, j] = np.hypot(xi - xj, yi - yj)

from scipy.sparse.csgraph import minimum_spanning_tree

mst = minimum_spanning_tree(dist_matrix)
# To get the edges and weights:
mst_edges = np.transpose(mst.nonzero())
total_weight = mst.sum()
print('MST total weight:', total_weight)
