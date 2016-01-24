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

variance_mean = []
variance_ci = []
variance_box = []

variance_na_mean = []
variance_na_ci = []
variance_na_box = []

variance_nab_mean = []
variance_nab_ci = []
variance_nab_box = []

n_s = [4, 10, 14]
n_rep = 30

p_g = [0.1+float(x)/10 for x in range(10)]

fig, ax = plt.subplots(figsize=(8.5, 5))

p = [] # plots

for i_s in n_s:
    # every number of subsystems
    compile_results('fig8n' + str(i_s), 'errVar', 'Fig8N'+str(i_s))
    
    f = open("../export/fig8n" + str(i_s) + '_data')
    lines = f.read().split(' ')
    i = 0

    variance_mean = []
    variance_na_mean = []

    for value in [False, True]:

        for p_g_i in p_g:

            var = []
            diff = []            

            for i_rep in range(n_rep):

                # every replication
                var_temp = []
                # var_na_temp = []
                for s in range(i_s):
                    # every subsystem in the replication
                    var_temp.append(float(lines[i]))
                    i += 1
                var.append(np.mean(var_temp))
                
            if value:                
                variance_mean.append(np.mean(var))                
            else:
                variance_na_mean.append(np.mean(var))                


    difference = []
    for i in range(len(variance_mean)):
        difference.append((variance_na_mean[i] - variance_mean[i])/variance_na_mean[i])
    
    p.append(ax.plot(p_g, difference, '.-'))

    f.close()

# remove_simdata()


# --- plotting --- #

ax.set_xlabel('State change probability ' + r'$\mathsf{P}[M=M_2]$')
ax.set_ylabel('Adaptation Gain')

ax.set_ylim((0, 1))

ax.grid(True)

# font size
font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

ax.legend((p[0][0], p[1][0], p[2][0]), (r'$N=4$', r'$N=10$', r'$N=14$'), loc=0, prop=font_prop)

ax.autoscale_view()

plt.show()