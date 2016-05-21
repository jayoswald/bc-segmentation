#!/usr/bin/env python
import sys
sys.path.append('../common')
import voxel
import numpy as np
import subprocess
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import scipy.optimize
rc = {'font.family': 'serif', 'font.size': 8, 'font.serif':['Times New Roman']}
matplotlib.rcParams.update(rc)

def main():
    data = []
    for r in [5,7,9,11,13,16,19,21]:
        voxels = voxel.make_sphere(r, h=1.0)
        voxel.write_voxels('spheres.txt', voxels)

        pid = subprocess.Popen('../../src/bc spheres.txt'.split(), stdout=subprocess.PIPE)
        out = pid.communicate()[0]
        data.append([float(len(voxels)), float(out.split('\n')[-2].split()[-1])])
        print data[-1]
    data = np.array(data)

    # Hack - not sure where this is coming from.
    data[:,1] /= 2.0

    plt.figure(figsize=(3,2))
    ax = plt.subplot(111)
    rc = {'mfc': 'w', 'ms': 5}
    ax.loglog(data[:,0], data[:,1], 'ko', **rc)


    res = lambda c: c[0]*data[3:,0]**c[1] - data[3:,1]
    fit = scipy.optimize.leastsq(res, [1.0, -1.0])
    print fit


    #plt.axis([0,0.5, 0, 9])
    plt.savefig('sphere.png', dpi=300)
   
 
if __name__ == '__main__':
    main()


