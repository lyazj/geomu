#!/usr/bin/env python3

import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

reader = csv.reader(open('../data/Bartol_pm14.csv'), delimiter='\t')

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

plt.contourf(*np.meshgrid(cos_theta, energy), density_p14, locator=ticker.LogLocator())
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$E_\nu$ [GeV]')
plt.yscale('log')
plt.colorbar()
plt.tight_layout()
plt.savefig('plot_Bartol_p14.pdf')

plt.clf()
plt.contourf(*np.meshgrid(cos_theta, energy), density_m14, locator=ticker.LogLocator())
plt.xlabel(r'$\cos\theta$')
plt.ylabel(r'$E_\nu$ [GeV]')
plt.yscale('log')
plt.colorbar()
plt.tight_layout()
plt.savefig('plot_Bartol_m14.pdf')
