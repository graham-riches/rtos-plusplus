# -*- coding: utf-8 -*-
"""
@brief
@author: Graham Riches
@date: Fri Jul 24 06:46:48 2020
@description
    quick script to plot out accelerometer data to compare filtering
    options using the DSP.
"""

import matplotlib.pyplot as plt
import pandas as pd
import sys


if __name__ == '__main__':
    if len(sys.argv) < 2:
        exit('USAGE: python serial_analyze.py [CSV_FILE]')
    filename = sys.argv[1]
    df = pd.read_csv(filename)
    df['time'] = df['time'] - df['time'].values[0]
    fig = plt.figure(figsize=(16, 9))
    xy_data = fig.add_subplot(2, 1, 1)
    xy_data.set_title('{} Acceleration Plots'.format(filename.strip('.csv')))
    xy_data.plot(df['time'], df['x'], '-k')
    xy_data.plot(df['time'], df['y'], '-r')
    xy_data.set_ylabel('Accel [g]')
    z_data = fig.add_subplot(2, 1, 2)
    z_data.plot(df['time'], df['z'], '-k')
    z_data.set_xlabel('Time [ms]')
    z_data.set_ylabel('Accel [g]')
    plt.show()
    fig.savefig('{}.png'.format(filename.strip('.csv')))
