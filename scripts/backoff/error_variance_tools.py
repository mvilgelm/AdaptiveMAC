"""
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail	Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich
"""
import sys
sys.path.append("..")

from preprocessing import compile_results, parse_sca
import matplotlib.pyplot as plt
import numpy as np
from os import listdir, system, getcwd, getenv
from os.path import isfile, join
import sys, re
import subprocess
import pandas as pd
from ci import getCI
from cycler import cycler


def plot_error_var():

    input = "../../src/results/"
    export = "../../export/testfile"

    configs = ["ErrorDependentExponent", "RegularExponent", "InverseExponent"]
    markers = {"ErrorDependentExponent": 's', "RegularExponent": '^', "InverseExponent": 'o'}
    colors = {"ErrorDependentExponent": 'r', "RegularExponent": 'b', "InverseExponent": 'g'}

    metric = "errVar"

    plt.figure()

    ax = plt.gca()

    for i, config in enumerate(configs):

        # see all the result files
        files = [f for f in listdir(input) if (isfile(join(input, f)) and (config in f) and ('.sca' in f))]

        print(files)

        files.sort(key=lambda x: int(re.findall('-(.+?).sca', x)[0]))

        print(files)

        # --- number of subsystems --- #
        n_min = 10
        n_max = 42
        step = 4
        n_s = [n_min + step * x for x in range(1, int((n_max - n_min) / step + 1))]

        df = pd.DataFrame()

        for f in files:
            df = df.append(parse_sca(filename=input+f))

        print(df)

        means = df.groupby('num_subsystems').mean()
        cis = df.groupby('num_subsystems').apply(lambda x: getCI(x, confidence=0.95))

        print(means)
        print(cis)

        means.plot(kind="line", yerr=cis.value, ax=ax, label=config, color=colors[config], marker=markers[config])

    plt.grid(True)

    legend = plt.legend(loc=0)
    for i, config in enumerate(configs):
        legend.get_texts()[i].set_text(config)

    # plt.yscale('log')
    plt.show()

    return


if __name__=="__main__":
    plot_error_var()