import matplotlib.pyplot as plt
import csv

temp_read_delay = 0.1  # in seconds

with open('openloop_step_response.txt', 'r') as f:
    changes = f.read().split('Heater ')[1:]

def get_x_values(changes: list[str]) -> list[float]:
    '''
    extracts x values aka time values from the .txt text
    '''
    sum_ = 0
    x_values = []
    for change in changes:
        num_x = change.count('Temperature')
        for _ in range(num_x):
            sum_ += temp_read_delay
            x_values.append(round(sum_, 3))

    return x_values

def get_y_values(changes: list[str]) -> list[float]:
    '''
    extracts y values of temperature sensor reading and heater state on/off
    '''
    ytemp = [] 
    yheater = []

    for change in changes:
        heater_False_found = change.find('False')
        heater_state = int(heater_False_found == -1)
        
        temp_txt_start_ind = change.index('Tem')
        temp_txt_end_ind = change.index('\n\n\n')
        temp_txt_splited = change[temp_txt_start_ind:temp_txt_end_ind].split('\n')

        ytemp.extend([float(value[13:]) for value in temp_txt_splited])
        yheater.extend([heater_state for _ in range(len(temp_txt_splited))])


    return ytemp, yheater


x = get_x_values(changes)
ytemp, yheat= get_y_values(changes)

### Plotting
fig, (temp_graph, heat_graph) = plt.subplots(2, 1)
temp_graph.plot(x, ytemp, color='blue')
heat_graph.plot(x, yheat, color='green')
plt.show()

### exporting csv for matlab to read
print(len(x) == len(ytemp) == len(yheat))  # All lists must be of equal size
print(len(x))
print(len(ytemp))
print(len(yheat))
with open('python_output.csv', 'w') as csv_file:
    csv_writer = csv.writer(csv_file, delimiter=',')

    csv_writer.writerow(x)
    csv_writer.writerow(ytemp)
    csv_writer.writerow(yheat)









