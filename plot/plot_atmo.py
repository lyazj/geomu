#!/usr/bin/env python3

import uproot
import numpy as np
import awkward as ak
import matplotlib as mpl
import matplotlib.pyplot as plt

events = uproot.open('../data/O16_1,999.ntuple.root:tree').arrays()

neutrino_logE_min = np.log(ak.min(events['neutrino_p4','fE']))
neutrino_logE_max = np.log(ak.max(events['neutrino_p4','fE']))
neutrino_E_bin = np.logspace(neutrino_logE_min, neutrino_logE_max, 41, base=np.e)

lepton_logE_min = np.log(ak.min(events['lepton_p4','fE']))
lepton_logE_max = np.log(ak.max(events['lepton_p4','fE']))
lepton_E_bin = np.logspace(lepton_logE_min, lepton_logE_max, 41, base=np.e)

cos_theta_min = -1
cos_theta_max = 1
cos_theta_bin = np.linspace(cos_theta_min, cos_theta_max, 41)

latex = {
    11: r'{e^-}',     -11: r'{e^+}',
    12: r'{\nu_e}',   -12: r'{\bar\nu_e}',
    13: r'{\mu^-}',   -13: r'{\mu^+}',
    14: r'{\nu_\mu}', -14: r'{\bar\nu_\mu}',
}

for neutrino_pid in [12, -12, 14, -14]:
    lepton_pid = neutrino_pid - np.sign(neutrino_pid)
    cutted_events = events[events['neutrino_pid'] == neutrino_pid]
    print('Neutrino %d: %d/%d events' % (neutrino_pid, len(cutted_events['weight']), len(events['weight'])))

    weight = cutted_events['weight']

    # Compute neutrino flux E and cos_theta.
    neutrino_E = cutted_events['neutrino_p4','fE']
    neutrino_p = cutted_events['neutrino_p4','fP']
    neutrino_cos_theta = neutrino_p['fZ'] / np.hypot(neutrino_p['fZ'], np.hypot(neutrino_p['fX'], neutrino_p['fY']))

    # Compute charged lepton flux E and cos_theta.
    lepton_E = cutted_events['lepton_p4','fE']
    lepton_p = cutted_events['lepton_p4','fP']
    lepton_cos_theta = lepton_p['fZ'] / np.hypot(lepton_p['fZ'], np.hypot(lepton_p['fX'], lepton_p['fY']))

    # Plot neutrino flux.
    plt.clf()
    plt.hist2d(neutrino_cos_theta.to_numpy(), neutrino_E.to_numpy(), (cos_theta_bin, neutrino_E_bin),
               weights=weight.to_numpy(), norm=mpl.colors.LogNorm())
    plt.xlabel(r'$\cos\theta$')
    plt.ylabel(f'$E_{latex[neutrino_pid]}$ [GeV]')
    plt.yscale('log')
    plt.colorbar()
    plt.tight_layout()
    plt.savefig('plot_neutrino_%d.pdf' % neutrino_pid)

    # Plot charged lepton flux.
    plt.clf()
    plt.hist2d(lepton_cos_theta.to_numpy(), lepton_E.to_numpy(), (cos_theta_bin, lepton_E_bin),
               weights=weight.to_numpy(), norm=mpl.colors.LogNorm())
    plt.xlabel(r'$\cos\theta$')
    plt.ylabel(f'$E_{latex[lepton_pid]}$ [GeV]')
    plt.yscale('log')
    plt.colorbar()
    plt.tight_layout()
    plt.savefig('plot_lepton_%d.pdf' % lepton_pid)
