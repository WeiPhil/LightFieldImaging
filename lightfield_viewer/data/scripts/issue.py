import os
from subprocess import call
import glob
import numpy as np
import matplotlib as plt
import matplotlib.pyplot as plt

alphaUV_0s = []
alphaUV_1s = []


alphaUs = [0.001, 0.005, 0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7]
alphaVs = [0.001, 0.005, 0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7]

outputDirectory = "./outputs"
imageOutput = "./matrix_outputs/"

def ordered_files(files):
    alphaVs.reverse()
    ordered_files = list()
    for alphaU in alphaUs:
        for alphaV in alphaVs:
            for f in files:
                if(str(alphaU).format("%0.3f")[2:].ljust(3, '0')+'_'+str(alphaV).format("%0.3f")[2:].ljust(3, '0') in f):
                    ordered_files.append(f)
                    continue
    alphaVs.reverse()
    return ordered_files


def clean_str(alphaUV):
    string = str(alphaUV)
    string = string.replace('[','')
    string = string.replace(']','')
    string = string.replace(', ','_')
    return string.replace('.','')


filesTemp = glob.glob('tmp/*.png')
files = ordered_files(filesTemp)
filename = 'approx_aniso2bounces.png'
width =int(np.sqrt(len(files)))
os.system("python3 imgmatrix.py "+" "+filename+" "+str(width)+" "+str(width)+" "+" ".join(files))
