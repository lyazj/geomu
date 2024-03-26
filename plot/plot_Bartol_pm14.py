#!/usr/bin/env python3

import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

reader = csv.reader(open('../data/Bartol_pm14.tsv'), delimiter='\t')

# Fetch cos_theta values.
for line in reader:
    cos_theta = np.array(line[1::2], dtype='float')
    break
energy = []
density_p14 = []
density_m14 = []
for line in reader:
    # Fetch energy value.
    energy.append(line[0])
    # Fetch muon and anti-muon neutrino densities.
    density_p14.append(line[1::2])
    density_m14.append(line[2::2])
energy, density_p14, density_m14 = map(lambda x: np.array(x, dtype='float'), (energy, density_p14, density_m14))

#plt.contourf(*np.meshgrid(cos_theta, energy), density_p14, locator=ticker.LogLocator())
#plt.xlabel(r'$\cos\theta$')
#plt.ylabel(r'$E_\nu$ [GeV]')
#plt.yscale('log')
#plt.colorbar()
#plt.tight_layout()
#plt.savefig('plot_Bartol_p14.pdf')
#
#plt.clf()
#plt.contourf(*np.meshgrid(cos_theta, energy), density_m14, locator=ticker.LogLocator())
#plt.xlabel(r'$\cos\theta$')
#plt.ylabel(r'$E_\nu$ [GeV]')
#plt.yscale('log')
#plt.colorbar()
#plt.tight_layout()
#plt.savefig('plot_Bartol_m14.pdf')

def format_1d(x):
    return [ '%.2e' % x for x in x ]

plt.clf()
density_p14_energy = density_p14.mean(1)
density_m14_energy = density_m14.mean(1)
plt.plot(energy, density_p14_energy, 'o-', label=r'$\nu_\mu$')
plt.plot(energy, density_m14_energy, 'o-', label=r'$\tilde\nu_\mu$')
plt.xlabel(r'$E_\nu$ [GeV]')
plt.ylabel(r'd$N_\nu$/d(log $E_\nu$) [cm$^{-2}$s$^{-1}$sr$^{-1}$]')
plt.xscale('log')
plt.yscale('log')
plt.legend()
plt.tight_layout()
plt.savefig('plot_Bartol_pm14_energy.pdf')

plt.clf()
density_p14_cos_theta = density_p14.mean(0)
density_m14_cos_theta = density_m14.mean(0)
plt.plot(cos_theta, density_p14_cos_theta, 'o-', label=r'$\nu_\mu$')
plt.plot(cos_theta, density_m14_cos_theta, 'o-', label=r'$\tilde\nu_\mu$')
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'd$N_\nu$/d(log $E_\nu$) [cm$^{-2}$s$^{-1}$sr$^{-1}$]')
plt.yscale('log')
plt.legend()
plt.tight_layout()
plt.savefig('plot_Bartol_pm14_cos_theta.pdf')
