#!/usr/bin/env python
# coding: utf-8

import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('out/percolation_thresholds.csv')
grouped_expanding = df.groupby('size').apply(lambda s: s.reset_index())['threshold'].expanding(min_periods=1)
expanding_mean = grouped_expanding.mean().unstack().T
expanding_std = grouped_expanding.std().unstack().T

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(20,10))
expanding_mean.plot(ax=ax1, grid=True, logx=True, title='Mean')
expanding_std.plot(ax=ax2, grid=True, logx=True, title='Standard deviation')

plt.savefig('plots.png')
plt.show()
