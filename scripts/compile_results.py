'''
This file is part of the Repeatability Evaluation submissions for the ACM HSCC'16.
Paper title: Adaptive Decentralized MAC for Event-Triggered Networked Control Systems

Code authors: 
Mikhail	Vilgelm	mikhail.vilgelm@tum.de

Copyright (c) Chair of Communication Networks, Technical University of Munich

Comments:
compile_results.py - quick & dirty script to convert the omnet++ output into the files 
necessary for the figure plotting scripts
'''


from os import listdir, system
from os.path import isfile, join
import sys, re

# output file - assuming command line argument is the figure
f_out_name = '../export/%s_data'%(sys.argv[1], )

# directory with the result files
path = '../src/results/'

# temp directory for processing results
tempdir = 'temp/'
system('mkdir '+tempdir)

# see all the result files
files = [f for f in listdir(path) if isfile(join(path, f))]

files.sort(key = lambda x: int(re.findall('-(.+?).sca', x)[0]))

# print(files)

# generate csv from result files
for f in files:
	system('scavetool s -p "errVar" -O temp/%s.csv -F csv %s'%(f,path+f))

# process all csv and compile one file out of them
csv_files = [f for f in listdir(tempdir) if isfile(join(tempdir, f))]

csv_files.sort(key = lambda x: int(re.findall('-(.+?).sca', x)[0]))

# print(csv_files)

f_out = open(f_out_name, 'w')

for f in csv_files:
	cf = open(tempdir+f)
	lines = cf.readlines()
	lines.pop(0)

	for line in lines:
		values = re.findall('errVar,(.+?)\r\n', line)		
		
		if len(values)==1:			
			# print('writing into file')
			f_out.write(str(values[0])+' ')
		else:
			print(values)

	cf.close()

f_out.close()

# cleanup
system('rm -r ' + tempdir)
system('rm ' + path+'*.sca')