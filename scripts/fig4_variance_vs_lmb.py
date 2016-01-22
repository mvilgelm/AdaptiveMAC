
import numpy as np

import scipy as sp
import scipy.stats

import matplotlib.pyplot as plt
import ci
import matplotlib

numPeriods = 200000

# --- 10-50 subsystems --- #

variance_mean = []
variance_ci = []

variance_box = []

lines = []


with open("/home/ga49zav/sim/Control/SimpleAdaptation/export/var_vs_lmb-1.csv") as f:
    for line in f:
        lines = line.split(' ')

print(len(lines))
print(lines[-1])
print(lines[-2])

n_s = [10, 16, 20]
n_rep = 30

# lmb = [(1+3*x) for x in range(6)]
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
        # if i_s==60:
        #    p1 = ax.plot([x for x in range(20)], var, 'bs-')

        results[n_s.index(i_s)].append(np.mean(var))
        results_box[n_s.index(i_s)].append(list(var))
        results_ci[n_s.index(i_s)].append(ci.getCI(var))
print i
f.close()

# --- plotting --- #

fig, ax = plt.subplots(figsize=(8.5, 5))

p = []

market_style = ['^', 'o', 's', 'x']

# for i_s in n_s:
#     if i_s == 20:
#         p.append(ax.plot(lmb[13:], results[n_s.index(i_s)][13:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
#     elif i_s == 16:
#         p.append(ax.plot(lmb[8:], results[n_s.index(i_s)][8:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
#     else:
#         p.append(ax.plot(lmb[0:], results[n_s.index(i_s)][0:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))

for i_s in n_s:
    if i_s == 20:
        p.append(ax.plot(lmb[13:], results[n_s.index(i_s)][13:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
    #elif i_s == 14:
    #    pass
        # p.append(ax.plot(lmb[7:], results[n_s.index(i_s)][7:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
    elif i_s == 16:
        p.append(ax.plot(lmb[8:], results[n_s.index(i_s)][8:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
    elif i_s == 10:
        p.append(ax.plot(lmb[0:], results[n_s.index(i_s)][0:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))
    # else:
        # p.append(ax.plot(lmb[0:], results[n_s.index(i_s)][0:], market_style[n_s.index(i_s)]+'-'))#, results_ci[n_s.index(i_s)][1:]))


# p0 = ax.boxplot(variance_box)
# p1 = ax.boxplot(variance_2_box)
# ax.set_xticklabels([str(x) for x in n_s])


# p0 = ax.errorbar(n_s, variance_mean, variance_ci)
# p1 = ax.errorbar(n_s, variance_2_mean, variance_2_ci)

# ax.set_xlabel('Threshold ' + r'$\Lambda^\prime$')
# ax.set_ylabel('Error variance ' r'$\mathsf{var}[e_k^i]$')


ax.set_xlabel(r'Threshold $\Lambda^\prime$')
ax.set_ylabel(r'Average $\mathsf{var}[e_k^i]$')


# ax.set_xlim((35, 105))
ax.set_ylim((0, 45))

ax.grid(True)
# font size
font_prop = matplotlib.font_manager.FontProperties(size=16)

for label in (ax.get_xticklabels() + ax.get_yticklabels()):
    label.set_fontproperties(font_prop)
    label.set_fontsize(14)

for item in [ax.xaxis.label, ax.yaxis.label]:
    item.set_fontsize(18)

# ax.legend((p[0][0], p[1][0], p[2][0]), (r'$N=10$', r'$N=10$', r'$N=14$'), loc=0, prop=font_prop)

ax.legend((p[0][0], p[1][0], p[2][0]), ('N=10', 'N=16', 'N=20'), loc=0, prop=font_prop)
ax.autoscale_view()
# plt.show()

plt.savefig('/home/ga49zav/Dropbox/Lkn/hscc2016/images/errvar_lmb.pdf', format='pdf', bbox_inches='tight')
# plt.savefig('/home/ga49zav/sim/CPNDFG/graphics/errvar_lmb.pdf', format='pdf', bbox_inches='tight')



