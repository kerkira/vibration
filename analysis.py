"""
Active Isolation System Analysis - built-in background

Essentially copied one from before
July 1, 2016

Kerkira Stockton

"""


# Importing #
import sys
sys.path.append('/Users/kerkira/Desktop/Email/')
import imp
import numpy as np 
import matplotlib.pyplot as plt
import numpy.polynomial.polynomial as poly
from scipy import signal as sg 
import pandas as pd
from pandas import DataFrame as df 
import analysisFunctions as af


file = []
data = []
acc = []
vel = []
frqacc = np.empty(16,dtype=np.ndarray)
frqvel = np.empty(16,dtype=np.ndarray)
welacc = np.empty(16,dtype=np.ndarray)
welvel = np.empty(16,dtype=np.ndarray)
print file

fileloc = "in_box/"
file.append(fileloc+"no_piezo_box.txt")
file.append(fileloc+"10hz.txt")
file.append(fileloc+"20hz.txt")
file.append(fileloc+"30hz.txt")
file.append(fileloc+"40hz.txt")
file.append(fileloc+"50hz.txt")
file.append(fileloc+"60hz.txt")
file.append(fileloc+"70hz.txt")
file.append(fileloc+"80hz.txt")
file.append(fileloc+"90hz.txt")
file.append(fileloc+"100hz.txt")
file.append(fileloc+"110hz.txt")
file.append(fileloc+"120hz.txt")
file.append(fileloc+"130hz.txt")
file.append(fileloc+"140hz.txt")
file.append(fileloc+"150hz.txt")
print file

first = 100									# first data point you want to read
last = 33000										# last data point you want to read



title = 'Seismometer response different freq in box'
save = 'compare_frequencies_inbox_170104'

sample_T = .002    #reading off the first time value 



for i in xrange(0,15):
	data.append(pd.read_csv(file[i],sep = '\t', header = None,skiprows = first,  nrows = last-first))
	data[i] = df.as_matrix(data[i])
	acc.append(data[i][:,0])
	vel.append(data[i][:,1])
	frqacc[i], welacc[i] = sg.welch(acc[i],1/sample_T,nperseg = 200,detrend = 'linear')
	frqvel[i], welvel[i] = sg.welch(vel[i],1/sample_T,nperseg = 200,detrend = 'linear')
	welacc[i] = np.sqrt(welacc[i])
	welvel[i] = np.sqrt(welvel[i])
	plt.loglog(frqacc[i],welacc[i],label = str(i*10) + 'hz')
	#plt.loglog(frqvel[i],welvel[i],label = 'Velocity at ' + str(i*10 + 20) + 'hz')
	print i, " done"

plt.loglog(frqacc[0],welacc[0],label = 'no piezo')

plt.legend(fontsize = 'small', loc = 'lower left')
plt.ylabel('Amplitude Spectral Density [volts/rt(Hz)]')
plt.xlabel('Frequency [Hz]')


plt.title(title)
plt.grid(True)



plt.savefig(save+ ".png", bbox_inches='tight',dpi = 100)
plt.show()