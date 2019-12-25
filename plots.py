#!/usr/bin/env python
# coding: utf-8

import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt

df = pd.read_csv('out/percolation_thresholds.csv')
grouped_by_size = df.groupby('size')

size_mean = grouped_by_size.apply(np.mean)
size_std = grouped_by_size.apply(np.std)
size_std['size'] = size_std.index

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20,10))
ax1.set_title('Mean threshold value')
ax2.set_title('Standard deviation')
sns.barplot('size', 'threshold', data=size_mean, ax=ax1)
sns.barplot('size', 'threshold', data=size_std, ax=ax2)

plt.savefig('plots.png')
plt.show()
