
import numpy as np

import matplotlib

import scipy as sp
import scipy.stats

import matplotlib.pyplot as plt
import ci

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
# n_rep = 100

p_g = [0.1+float(x)/10 for x in range(10)]
# print(p_g)



fig, ax = plt.subplots(figsize=(8.5, 5))

p = [] # plots

for i_s in n_s:
    # every number of subsystems

    f = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/"+str(i_s) +"-adaptation-p-1.csv")
    lines = f.read().split(' ')

    i = 0

    difference = []

    variance_mean = []
    variance_na_mean = []

    for value in [False, True]:

        for p_g_i in p_g:

            var = []
            diff = []
            # var_na = []

            for i_rep in range(n_rep):

                # print(str(i_s) + ' ' + str(value) + ' ' + str(p_g_i) + ' ' + str(i_rep))
                # every replication
                var_temp = []
                # var_na_temp = []
                for s in range(i_s):
                    # every subsystem in the replication
                    var_temp.append(float(lines[i]))
                    # var_na_temp.append(float(lines1[i]))
                    i += 1
                var.append(np.mean(var_temp))
                # var_na.append(np.mean(var_na_temp))

            if value:
                # variance_box.append(list(var))
                variance_mean.append(np.mean(var))
                # variance_ci.append(ci.getCI(var))
            else:
                # variance_na_box.append(list(var))
                variance_na_mean.append(np.mean(var))
                # variance_na_ci.append(ci.getCI(var))
    # p.append(ax.plot(p_g, variance_mean))
    # p.append(ax.plot(p_g, variance_na_mean))

    difference = []
    difference_ci = []
    for i in range(len(variance_mean)):
        difference.append((variance_na_mean[i] - variance_mean[i])/variance_na_mean[i])
        # difference_ci.append((variance_na_ci[i] - variance_ci[i])/variance_na_ci[i])
    

    #p.append(ax.errorbar(p_g, difference, yerr=difference_ci))
    p.append(ax.plot(p_g, difference, '.-'))

f.close()
# f1.close()

# --- plotting --- #

# p0 = ax.boxplot(variance_box)
# p1 = ax.boxplot(variance_na_box)
# p1 = ax.boxplot(variance_2_box)
# ax.set_xticklabels([str(x) for x in n_s])
# p0 = ax.errorbar(n_s, variance_mean, yerr=variance_ci, fmt='-o')# , 'bs-')

# p1 = ax.errorbar(n_s, variance_na_mean, yerr=variance_na_ci, fmt='-^')# , 'g^-')


ax.set_xlabel('State change probability ' + r'$\mathsf{P}[M=M_2]$')
ax.set_ylabel('Adaptation Gain')

# ax.set_yscale('log')#, nonposy="clip")

# ax.set_xlim((35, 105)
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