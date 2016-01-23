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
import os
from preprocessing import compile_results, remove_simdata

# variance_mean = []
# variance_ci = []
variance_box = []

# preprocess the data
compile_results('fig3', 'errVar', 'Fig3')

# open results of preprocessing
f = open("../export/fig3_data")

lines = f.read().split(' ')

# --- 2-24 subsystems --- #
n_s = [2*x for x in range(1, 13)]
n_rep = 30

i = 0

fig, ax = plt.subplots(figsize=(8.5, 5))

for i_s in n_s:
    # every number of subsystems
    var = []
    for i_rep in range(n_rep):
        # every replication
        var_temp = []
        for s in range(i_s):
            # every subsystem in the replication
            var_temp.append(float(lines[i]))
            i += 1
        var.append(np.mean(var_temp))

    variance_box.append(list(var))
    # variance_mean.append(np.mean(var))
    # variance_ci.append(ci.getCI(var))

# --- cleanup --- #
f.close()
os.system('rm ../export/fig3_data')
remove_simdata()

# --- plotting --- #

p0 = ax.boxplot(variance_box)

ax.set_xticklabels([str(x) for x in n_s])

ax.set_xlabel('Number of subsystems ' + r'$N$')
ax.set_ylabel('Average ' r'$\mathsf{var}[e_k^i]$')
ax.set_yscale('log', nonposy="clip")

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