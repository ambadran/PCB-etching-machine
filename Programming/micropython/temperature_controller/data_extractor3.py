import matplotlib.pyplot as plt
import csv

temp_read_delay = 0.1  # in seconds

# Reading log file
with open('openloop_step_response2.txt', 'r') as f:
    temp_readings = [float(value[13:]) for value in f.read().split('\n')[:-1]]


### exporting csv for matlab to read
with open('python_output2.csv', 'w') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=',')

    csv_writer.writerow(temp_readings)







