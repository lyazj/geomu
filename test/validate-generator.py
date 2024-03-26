#!/usr/bin/env python3

import os
import csv
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

reader = csv.reader(open('../data/Bartol_pm14.tsv'), delimiter='\t')

# Fetch cos_theta values.
for line in reader:
    cos_theta = np.array(line[1::2], dtype='float')
    break
energy = []
for line in reader:
    # Fetch energy value.
    energy.append(line[0])
energy = np.array(energy, dtype='float')
log_energy = np.log(energy)
cos_theta = cos_theta[::-1]

data = np.array([r.split() for r in os.popen('./generator-test').read().strip().split('\n')], dtype='float')
data[:,0] = np.log(data[:,0])
os.makedirs('generator', 0o755, True)

plt.hist(data[:,2], 50, histtype='step')
plt.xlabel(r'Weight [cm$^{-2}$s$^{-1}$]')
plt.ylabel(r'Events')
plt.yscale('log')
plt.tight_layout()
plt.savefig('generator/generator-weight.pdf')

cos_theta = np.linspace(cos_theta[0], cos_theta[-1], 51)
log_energy = np.linspace(log_energy[0], log_energy[-1], 51)
plt.clf()
hst, _, _, _ = plt.hist2d(data[:,1], data[:,0], (cos_theta, log_energy), weights=data[:,2] * (data[:,3] == 0) / data.shape[0], norm=mpl.colors.LogNorm())
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$\log(E_\nu/$GeV$)$')
plt.colorbar()
plt.tight_layout()
plt.savefig('generator/generator-histogram-p14.pdf')

plt.clf()
plt.contourf(
    *np.meshgrid((cos_theta[1:] + cos_theta[:-1]) / 2.0, (log_energy[1:] + log_energy[:-1]) / 2.0),
    hst.T / (2 * np.pi * (cos_theta[1:] - cos_theta[:-1]) * (log_energy[1:] - log_energy[:-1])),
    locator=ticker.LogLocator()
)
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$\log(E_\nu/$GeV$)$')
plt.colorbar()
plt.tight_layout()
plt.savefig('generator/generator-contourf-p14.pdf')


cos_theta = np.linspace(cos_theta[0], cos_theta[-1], 51)
log_energy = np.linspace(log_energy[0], log_energy[-1], 51)
plt.clf()
hst, _, _, _ = plt.hist2d(data[:,1], data[:,0], (cos_theta, log_energy), weights=data[:,2] * (data[:,3] != 0) / data.shape[0], norm=mpl.colors.LogNorm())
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$\log(E_{\tilde\nu}/$GeV$)$')
plt.colorbar()
plt.tight_layout()
plt.savefig('generator/generator-histogram-m14.pdf')

plt.clf()
plt.contourf(
    *np.meshgrid((cos_theta[1:] + cos_theta[:-1]) / 2.0, (log_energy[1:] + log_energy[:-1]) / 2.0),
    hst.T / (2 * np.pi * (cos_theta[1:] - cos_theta[:-1]) * (log_energy[1:] - log_energy[:-1])),
    locator=ticker.LogLocator()
)
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$\log(E_{\tilde\nu}/$GeV$)$')
plt.colorbar()
plt.tight_layout()
plt.savefig('generator/generator-contourf-m14.pdf')
