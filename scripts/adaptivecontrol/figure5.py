'''
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact: 
Mikhail Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
'''


import numpy as np
import matplotlib
import scipy as sp
import scipy.stats
import matplotlib.pyplot as plt
import ci

from preprocessing import compile_results, remove_simdata


# --- throughput --- #
compile_results('fig5_throughput', 'Throughput', 'Fig5')
f = open("../export/fig5_throughput_data")
lines = f.read().split(' ')

# --- accesses --- #
compile_results('fig5_access', 'Access', 'Fig5')
f1 = open("../export/fig5_access_data")
lines1 = f1.read().split(' ')

# --- collisions --- #
compile_results('fig5_collisions', 'Collisions', 'Fig5')
f2 = open("../export/fig5_collisions_data")
lines2 = f2.read().split(' ')

# --- parameters --- #
n_s = [2*x for x in range(1, 21)]
n_rep = 30

i = 0

fig, ax = plt.subplots(figsize=(8.5, 6))

throughput = []
throughput_box = []
throughput_ci = []

access = []
access_box = []
access_ci = []

collisions = []
collisions_box = []
collisions_ci = []

for i_s in n_s:
    # every number of subsystems
    thr = []
    acc = []
    col = []
    for i_rep in range(n_rep):
        # every replication
        thr.append(float(lines[i]))
        acc.append(float(lines1[i]))
        col.append(float(lines2[i]))
        i += 1

    throughput_box.append(list(thr))
    throughput.append(np.mean(thr))
    throughput_ci.append(ci.getCI(thr))

    access.append(np.mean(acc))
    access_box.append(list(acc))
    access_ci.append(ci.getCI(acc))

    collisions.append(np.mean(col))
    collisions_box.append(list(col))
    collisions_ci.append(ci.getCI(col))


# --- cleanup --- #
f.close()
f1.close()
f2.close()
# remove_simdata()

# --- plotting --- #

ax1 = ax.twinx()

p0 = ax.errorbar(n_s, throughput, yerr=throughput_ci, fmt='-o', color='blue')

collision_rate = []
for i in range(len(access)):
    collision_rate.append(collisions[i]/access[i])

# p1 = ax1.plot(n_s, collision_rate, 'r--.')
p1 = ax1.errorbar(n_s, collision_rate, yerr=collisions_ci, fmt='--.', color='red')

ax1.set_ylabel('Ratio of collided packets', color='r')
for tl in ax1.get_yticklabels():
    tl.set_color('r')

ax.set_xlabel(r'Number of sub-systems $N$')
ax.set_ylabel('Throughput')

# font size
font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

ax.grid(True)

ax.autoscale_view()
plt.show()

