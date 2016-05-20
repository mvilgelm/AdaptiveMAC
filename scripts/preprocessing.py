'''
This file is part of the Repeatability Evaluation submission for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Contact:
Mikhail	Vilgelm
mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich

Comments:
compile_results.py - quick & dirty script to convert the omnet++ output into the files
necessary for the figure plotting scripts
'''

from os import listdir, system, getcwd, getenv
from os.path import isfile, join
import sys, re
import subprocess
import pandas as pd

sys.path.append(getenv("HOME") + '/omnetpp-5.0/bin')


def run_bash(cmd):
    p = subprocess.Popen([cmd])
    for line in p.stdout.readlines():
        print(line)



def compile_results(f_out_name, metric, config, in_directory):
    print('########## Starting preprocessing')
    print(getcwd())

    # directory with the result files
    path = in_directory

    # temp directory for processing results
    tempdir = 'temp/'
    system('mkdir ' + tempdir)

    # see all the result files
    files = [f for f in listdir(path) if (isfile(join(path, f)) and (config in f))]

    files.sort(key=lambda x: int(re.findall('-(.+?).sca', x)[0]))

    print(getcwd())
    # generate csv from result files
    for f in files:
        print(path + f)
        cmd = "scavetool s -p \"%s\" -O temp/%s.csv -F csv %s" % (metric, f, path + f)
        run_bash(cmd)

    # process all csv and compile one file out of them
    csv_files = [f for f in listdir(tempdir) if isfile(join(tempdir, f))]

    csv_files.sort(key=lambda x: int(re.findall('-(.+?).sca', x)[0]))

    # print(csv_files)

    f_out = open(f_out_name + '_data', 'w')

    for f in csv_files:
        cf = open(tempdir + f)
        lines = cf.readlines()
        lines.pop(0)

        for line in lines:
            values = re.findall(metric + ',(.+?)\n', line)

            if len(values) == 1:
                # print('writing into file')
                f_out.write(str(float(values[0])) + ' ')
            else:
                print(line)
                print('incorrect value')

        cf.close()

    f_out.close()

    # --- cleanup --- #
    system('rm -r ' + tempdir)
    # remove_simdata()
    print('########## Finished preprocessing')
    return True


def remove_simdata():
    system('rm ' + '../src/results/*.sca')


def parse_sca(filename, param=None):
    """
    Parse a sca file and return a dataframe
    :param filename:
    :param n_s:
    :return:
    """

    # parse to find number of subsystems
    with open(filename) as f:
        for line in f:
            if line.startswith('scalar'):
                lines = line.split('\t')
                n_s = int(lines[-1])
                break

    df = pd.read_csv(filename, sep='\t', header=None, skiprows=31)
    df = df.rename(columns={0: 'num_subsystems', 1: 'metric', 2: 'value'})
    df.num_subsystems = df.num_subsystems.apply(lambda x: n_s)

    return df





if __name__ == '__main__':
    # here comes unit test
    parse_sca('../src/results/Debugging-0.sca', 10)
