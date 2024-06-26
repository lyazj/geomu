#!/usr/bin/env python3

import os
import numpy as np
import matplotlib.pyplot as plt

data = np.array([r.split() for r in os.popen('./sampler-test').read().strip().split('\n')], dtype='float')

plt.hist(data[:,2], 50, histtype='step')
plt.tight_layout()
plt.show()

hst, _, _, _ = plt.hist2d(data[:,0], data[:,1], (2, 2), weights=data[:,2] / data.shape[0])
print(hst)  # expect: [[4, 6], [10, 12]]
plt.colorbar()
plt.tight_layout()
plt.show()

plt.hist2d(data[:,0], data[:,1], (10, 10), weights=data[:,2] / data.shape[0])
plt.colorbar()
plt.tight_layout()
plt.show()  # expect: yield monotonously rises from LL to UR
