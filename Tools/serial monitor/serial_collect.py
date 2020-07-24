# -*- coding: utf-8 -*-
"""
@brief
@author: Graham Riches
@date: Fri Jul 24 06:23:37 2020
@description
    Quick script to collect serial accelerometer data from the stm32f4
    discovery board to optimize the accelerometer filter parameters
"""

import sys
import csv
import time
import serial


RUN_TIME = 10


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('USAGE: python serial_collect.py [COMxx] [FILENAME]')
        exit()

    port = sys.argv[1]
    filename = sys.argv[2]

    ser = serial.Serial(port, baudrate=115200, timeout=1)
    print('Connected: Starting data collection...')

    start_time = time.time()
    with open('{}.csv'.format(filename), 'w', newline='\n') as csv_file:
        writer = csv.writer(csv_file, delimiter=',')
        # print out the column headers
        headers = ['time', 'x', 'y', 'z']
        writer.writerow(headers)
        while (time.time() - start_time) < RUN_TIME:
            line = ser.readline()
            items = line.decode().strip('\r\n').split(' ')
            if len(items) == 4:
                writer.writerow(items)
    print('Done')
