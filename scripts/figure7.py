
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

variance_na_mean = []
variance_na_ci = []
variance_na_box = []

variance_nab_mean = []
variance_nab_ci = []
variance_nab_box = []

f = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/adaptive-1.csv")
lines = f.read().split(' ')

f1 = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/non-adaptive-1.csv")
lines1 = f1.read().split(' ')

f2 = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/non-adaptive-b-1.csv")
lines2 = f2.read().split(' ')

n_s = [4, 6, 8, 10, 12, 14, 16]
# n_s = [60]
# n_s = [26*x for x in range(1, 6)]
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

f.close()
f1.close()
f2.close()

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

# p0 = ax.boxplot(variance_box)
# p1 = ax.boxplot(variance_na_box)
# p1 = ax.boxplot(variance_2_box)
# ax.set_xticklabels([str(x) for x in n_s])
p0 = ax.errorbar(n_s, variance_mean, yerr=variance_ci, fmt='-o')# , 'bs-')
p1 = ax.errorbar(n_s, variance_na_mean, yerr=variance_na_ci, fmt='-^')# , 'g^-')
p2 = ax.errorbar(n_s, variance_nab_mean, yerr=variance_nab_ci, fmt='-s')# , 'g^-')


ax.set_xlabel(r'Number of sub-systems $N$')
ax.set_ylabel(r'Average ' r'$\mathsf{var}[e_k^i]$')

ax.set_yscale('log')#, nonposy="clip")

# ax.set_xlim((35, 105)
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
# plt.show()

plt.savefig('/home/ga49zav/Dropbox/Lkn/hscc2016/images/adaptation.pdf', format='pdf', bbox_inches='tight')

