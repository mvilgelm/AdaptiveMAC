# -*- coding: utf-8 -*-

import numpy as np
import scipy as sp
import scipy.stats
import matplotlib.pyplot as plt


def getCI(data, confidence=.95):
    
    n = len(data)
    s = np.std(data)

    se = s/np.sqrt(n)

    ci = se * sp.stats.t._ppf((1+confidence)/2., n-1)

    return ci
    

if __name__=='__main__':
    
    data = [np.random.normal(0,1) for x in range(100)]
    
    ci = getCI(data)
    
    plt.figure()    
    plt.errorbar([x for x in range(100)], data, ci)