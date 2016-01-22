import matplotlib.pyplot as plt

import numpy as np

import matplotlib

import scipy as sp
import scipy.stats

import matplotlib.pyplot as plt
import ci


# --- throughput --- #
f = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/throughput-1.csv")
lines = f.read().split(' ')

# --- accesses --- #
f1 = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/access-1.csv")
lines1 = f1.read().split(' ')

# --- collisions --- #
f2 = open("/home/ga49zav/sim/Control/SimpleAdaptation/export/collisions-1.csv")
lines2 = f2.read().split(' ')

n_s = [2*x for x in range(1, 21)]
# n_s = [60]
# n_s = [26*x for x in range(1, 6)]
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

    # if i_s==60:
    #    p1 = ax.plot([x for x in range(20)], var, 'bs-')
    throughput_box.append(list(thr))
    throughput.append(np.mean(thr))
    throughput_ci.append(ci.getCI(thr))

    access.append(np.mean(acc))
    access_box.append(list(acc))
    access_ci.append(ci.getCI(acc))

    collisions.append(np.mean(col))
    collisions_box.append(list(col))
    collisions_ci.append(ci.getCI(col))

# --- plotting --- #

ax1 = ax.twinx()

# p0 = ax.boxplot(throughput_box)
# p1 = ax.boxplot(variance_2_box)
# ax.set_xticklabels([str(x) for x in n_s])
p0 = ax.errorbar(n_s, throughput, yerr=throughput_ci, fmt='-o', color='blue')# , 'bs-')


# p1 = ax1.errorbar(n_s, [access], yerr=access_ci, fmt='--.', color='red')# , 'g^-')

# p2 = ax1.errorbar(n_s, collisions, yerr=collisions_ci, fmt='--^', color='red')# , 'g^-')

collision_rate = []
for i in range(len(access)):
    collision_rate.append(collisions[i]/access[i])

# p1 = ax1.errorbar(n_s, , yerr=access_ci, fmt='--.', color='red')# , 'g^-')

p1 = ax1.plot(n_s, collision_rate, 'r--.')

ax1.set_ylabel('Ratio of collided packets', color='r')
for tl in ax1.get_yticklabels():
    tl.set_color('r')

ax.set_xlabel(r'Number of sub-systems $N$')
ax.set_ylabel('Throughput')

# ax1.legend((p1[0], p2[0]), ('Accesses', 'Collisions'))

# ax.set_yscale('log', nonposy="clip")

# ax.set_xlim((35, 105)
# ax.set_ylim(


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

plt.savefig('/home/ga49zav/Dropbox/Lkn/hscc2016/images/throughput_access.pdf', format='pdf', bbox_inches='tight')

f.close()
f1.close()