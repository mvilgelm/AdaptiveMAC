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


from os import listdir, system
from os.path import isfile, join
import sys, re


def compile_results(f_out_name, metric, config):
	print('########## Starting preprocessing')

	# directory with the result files
	path = '../src/results/'

	# temp directory for processing results
	tempdir = 'temp/'
	system('mkdir ' + tempdir)

	# see all the result files
	files = [f for f in listdir(path) if (isfile(join(path, f)) and (config in f))]

	files.sort(key = lambda x: int(re.findall('-(.+?).sca', x)[0]))

	# print(files)

	# generate csv from result files
	for f in files:
		system('scavetool s -p "%s" -O temp/%s.csv -F csv %s'%(metric, f, path+f))

	# process all csv and compile one file out of them
	csv_files = [f for f in listdir(tempdir) if isfile(join(tempdir, f))]

	csv_files.sort(key = lambda x: int(re.findall('-(.+?).sca', x)[0]))

	# print(csv_files)

	f_out = open('../export/'+f_out_name+'_data', 'w')

	for f in csv_files:
		cf = open(tempdir+f)
		lines = cf.readlines()
		lines.pop(0)

		for line in lines:
			values = re.findall(metric+',(.+?)\r\n', line)		
			
			if len(values)==1:			
				# print('writing into file')
				f_out.write(str(values[0])+' ')
			else:
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


if __name__ == '__main__':
	# here comes unit test
	pass
