'''
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact: 
Mikhail Vilgelm 
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
'''

import numpy as np
import scipy as sp
import scipy.stats
import matplotlib.pyplot as plt
import ci
import matplotlib
from preprocessing import compile_results



# --- preprocessing --- #

compile_results('fig4')

lines = []

with open("../export/fig4_data") as f:
    for line in f:
        lines = line.split(' ')


# --- setting parameters --- #

n_s = [10, 16, 20]
n_rep = 30

lmb = [(0.0+.1*x) for x in range(41)]

i = 0

results = []
results_box = []
results_ci = []

for i_s in n_s:
    # every number of subsystems
    results.append([])
    results_ci.append([])
    results_box.append([])

    for lmb_v in lmb:
        # every lambda value
        var = []
        for i_rep in range(n_rep):
            # every replication
            var_temp = []
            for s in range(i_s):
                # every subsystem in the replication
                var_temp.append(float(lines[i]))
                i += 1
            var.append(np.mean(var_temp))

        results[n_s.index(i_s)].append(np.mean(var))
        results_box[n_s.index(i_s)].append(list(var))
        results_ci[n_s.index(i_s)].append(ci.getCI(var))
print i
f.close()

# --- plotting --- #

fig, ax = plt.subplots(figsize=(8.5, 5))

p = []

market_style = ['^', 'o', 's', 'x']

for i_s in n_s:
    if i_s == 20:
        p.append(ax.plot(lmb[13:], results[n_s.index(i_s)][13:], market_style[n_s.index(i_s)]+'-'))       
    elif i_s == 16:
        p.append(ax.plot(lmb[8:], results[n_s.index(i_s)][8:], market_style[n_s.index(i_s)]+'-'))
    elif i_s == 10:
        p.append(ax.plot(lmb[0:], results[n_s.index(i_s)][0:], market_style[n_s.index(i_s)]+'-'))

ax.set_xlabel(r'Threshold $\Lambda^\prime$')
ax.set_ylabel(r'Average $\mathsf{var}[e_k^i]$')


ax.set_ylim((0, 45))

ax.grid(True)

# font size
font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

ax.legend((p[0][0], p[1][0], p[2][0]), ('N=10', 'N=16', 'N=20'), loc=0, prop=font_prop)
ax.autoscale_view()

plt.show()


