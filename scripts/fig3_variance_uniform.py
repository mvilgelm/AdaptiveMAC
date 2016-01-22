# -*- coding: utf-8 -*-

# -*- coding: utf-8 -*-

import numpy as np

import matplotlib

import scipy as sp
import scipy.stats

import matplotlib.pyplot as plt
import ci


numPeriods = 200000

# --- 10-50 subsystems --- #

variance_mean = []
variance_ci = []

variance_box = []

f = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/variance-1.csv")
lines = f.read().split(' ')

n_s = [2*x for x in range(1, 13)]
# n_s = [60]
# n_s = [26*x for x in range(1, 6)]
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
    # if i_s==60:
    #    p1 = ax.plot([x for x in range(20)], var, 'bs-')
    variance_box.append(list(var))
    variance_mean.append(np.mean(var))
    variance_ci.append(ci.getCI(var))

f.close()

# --- 60 subsystems, 100 runs --- #

# variance_2_mean = []
# variance_2_ci = []
#
# variance_2_box = []
#
# # f = open("/home/ga49zav/Control/DecentralizedScheduling/data/channel_state/variance")
# # lines = f.readlines()
#
# n_rep = 30
# i_s = [40, 60, 80, 100]
#
# for i_s in n_s:
#     # every number of subsystems
#     var = []
#     for i_rep in range(n_rep):
#         # every replication
#         var_temp = []
#         for s in range(i_s):
#             # every subsystem in the replication
#             var_temp.append(float(lines[i]))
#             i += 1
#         var.append(np.mean(var_temp))
#     # if i_s==60:
#     #    p1 = ax.plot([x for x in range(20)], var, 'bs-')
#
#     variance_2_box.append(list(var))
#     variance_2_mean.append(np.mean(var))
#     variance_2_ci.append(ci.getCI(var))
# f.close()


# --- plotting --- #


p0 = ax.boxplot(variance_box)
# p1 = ax.boxplot(variance_2_box)
ax.set_xticklabels([str(x) for x in n_s])
# p0 = ax.errorbar(n_s, variance_mean, variance_ci)# , 'bs-')
# p1 = ax.errorbar(n_s, variance_2_mean, variance_2_ci)# , 'g^-')

ax.set_xlabel('Number of subsystems ' + r'$N$')
ax.set_ylabel('Average ' r'$\mathsf{var}[e_k^i]$')

ax.set_yscale('log', nonposy="clip")

# ax.set_xlim((35, 105)
# ax.set_ylim((10, 30))

# font size
font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

ax.grid(True)

# ax.legend((p0[0]), ('fixed threshold'))
# ax.set_yscale('log')

ax.autoscale_view()
# plt.show()

plt.savefig('/home/ga49zav/Dropbox/Lkn/hscc2016/images/errvar0.pdf', format='pdf', bbox_inches='tight')

