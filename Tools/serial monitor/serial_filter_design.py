# -*- coding: utf-8 -*-
"""
@brief
@author: Graham Riches
@date: Sat Jul 25 06:32:00 2020
@description
    filter design using scipy
"""
import json
import pandas as pd
import numpy as np
from scipy.signal import firwin, freqz
import matplotlib.pyplot as plt

NUM_TAPS = 20
FS = 400
CUTOFF = 20

filename = 'no_filt400.csv'
df = pd.read_csv(filename)

# design the filter
filt_coeffs = firwin(NUM_TAPS, CUTOFF, fs=FS)
w, h = freqz(filt_coeffs, fs=FS)
filt_signal = np.convolve(df['x'], filt_coeffs, mode='same')

# dump the filter coeffiencts to json file
with open('filter_coefficents.json', 'w') as json_file:
    data = json.dumps(json.loads(json.dumps(filt_coeffs.tolist()),
                                 parse_float=lambda x: round(float(x), 6)))
    # json.dumps(filt_coeffs.tolist())
    json_file.write(data)

fig = plt.figure(figsize=(16, 9))
filt = fig.add_subplot(2, 1, 1)
filt.plot(filt_coeffs, '-ob')
filt.set_title('FIR Filter Coefficients {} Taps'.format(NUM_TAPS))
filt.grid(True)

resp = fig.add_subplot(2, 1, 2)
resp.plot(w, 20 * np.log10(abs(h)), 'b')
resp.set_xlabel('Frequency [Hz]')
resp.set_ylabel('Amplitube [dB]')
resp.grid(True)
plt.show()


# plot out some filtered data
df['time'] = df['time'] - df['time'].values[0]
fig = plt.figure(figsize=(16, 9))
xy_data = fig.add_subplot(1, 1, 1)
xy_data.set_title('{} Acceleration Plots'.format(filename.strip('.csv')))
xy_data.plot(df['time'], df['x'], '-k')
xy_data.plot(df['time'], filt_signal, '-r')
xy_data.set_ylabel('Accel [g]')
xy_data.set_xlabel('Time [ms]')
plt.show()
