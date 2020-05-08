#!/usr/bin/env python3
# coding: utf-8

import numpy as np
import pandas as pd
import seaborn as sns
from matplotlib import pyplot as plt

df = pd.read_csv('out/percolation_thresholds.csv')
grouped_by_size = df.groupby('size')

size_mean = grouped_by_size.apply(np.mean)
size_mean['size'] = size_mean.index
size_std = grouped_by_size.apply(np.std)
size_std['size'] = size_std.index

sns.set(font_scale=3)
fig, ax = plt.subplots(1, 1, figsize=(10, 10))
sns.barplot('size', 'threshold', data=size_mean, ax=ax)
ax.set_title('')
ax.set(xlabel='', ylabel='')
plt.savefig('mean-plot.png')
fig.show()

sns.set(font_scale=3)
fig, ax = plt.subplots(1, 1, figsize=(10, 10))
sns.barplot('size', 'threshold', data=size_std, ax=ax)
ax.set_title('')
ax.set(xlabel='', ylabel='')
plt.savefig('std-plot.png')
plt.show()
