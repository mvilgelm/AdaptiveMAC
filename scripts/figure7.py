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


# --- variables --- #
variance_mean = []
variance_ci = []
variance_box = []

variance_na_mean = []
variance_na_ci = []
variance_na_box = []

variance_nab_mean = []
variance_nab_ci = []
variance_nab_box = []

# --- preprocessing --- #

compile_results('fig7adapt', 'errVar', 'Fig7Adapt')
f = open("../export/fig7adapt_data")
# f = open("../export/fig7_adapt_data-1.csv")
lines = f.read().split(' ')

compile_results('fig7nadapt_a', 'errVar', 'Fig7NonAdaptA')
f1 = open("../export/fig7nadapt_a_data")
# f1 = open("../export/fig7_nadapt_a_data-1.csv")
lines1 = f1.read().split(' ')

compile_results('fig7nadapt_b', 'errVar', 'Fig7NonAdaptB')
f2 = open("../export/fig7nadapt_b_data")
# f2 = open("../export/fig7_nadapt_b_data-1.csv")
lines2 = f2.read().split(' ')

n_s = [4, 6, 8, 10, 12, 14, 16]
n_rep = 30

i = 0

fig, ax = plt.subplots(figsize=(8.5, 5))

for i_s in n_s:
    # every number of subsystems
    var = []
    var_na = []
    var_nab = []
    for i_rep in range(n_rep):
        # every replication
        var_temp = []
        var_na_temp = []
        var_nab_temp = []
        for s in range(i_s):
            # every subsystem in the replication
            var_temp.append(float(lines[i]))
            var_na_temp.append(float(lines1[i]))
            var_nab_temp.append(float(lines2[i]))
            i += 1
        var.append(np.mean(var_temp))
        var_na.append(np.mean(var_na_temp))
        var_nab.append(np.mean(var_nab_temp))

    # if i_s==60:
    #    p1 = ax.plot([x for x in range(20)], var, 'bs-')
    variance_box.append(list(var))
    variance_mean.append(np.mean(var))
    variance_ci.append(ci.getCI(var))

    variance_na_box.append(list(var_na))
    variance_na_mean.append(np.mean(var_na))
    variance_na_ci.append(ci.getCI(var_na))

    variance_nab_box.append(list(var_nab))
    variance_nab_mean.append(np.mean(var_nab))
    variance_nab_ci.append(ci.getCI(var_nab))

# --- cleanup --- #

f.close()
f1.close()
f2.close()
remove_simdata()

# --- plotting --- #

p0 = ax.errorbar(n_s, variance_mean, yerr=variance_ci, fmt='-o')# , 'bs-')
p1 = ax.errorbar(n_s, variance_na_mean, yerr=variance_na_ci, fmt='-^')# , 'g^-')
p2 = ax.errorbar(n_s, variance_nab_mean, yerr=variance_nab_ci, fmt='-s')# , 'g^-')


ax.set_xlabel(r'Number of sub-systems $N$')
ax.set_ylabel(r'Average ' r'$\mathsf{var}[e_k^i]$')

ax.set_yscale('log')

ax.set_ylim((0, 600))

ax.grid(True)

# font size

font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

ax.legend((p2[0], p1[0], p0[0]), ('Non-Adapt. (B)', 'Non-Adapt. (A)', 'Adaptive'), loc=0, prop=font_prop)


ax.autoscale_view()

plt.show()