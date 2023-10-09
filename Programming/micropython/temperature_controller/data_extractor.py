import matplotlib.pyplot as plt

# Reading Raw Values as list of dictionary
with open('log.txt', 'r') as f:
    dicts = f.read().split('Current PID values:\n')
    dicts = [eval(dict_) for dict_ in dicts[1:-1]]

# Extracting Temperature data
x = list(range(len(dicts)))
temp_y = [dict_['_last_input'] for dict_ in dicts]
set_point = dicts[0]['setpoint']
heat_y = [dict_['_last_output'] for dict_ in dicts]


# Plotting
fig, (temp_graph, heat_graph) = plt.subplots(2, 1)

temp_graph.plot(x, temp_y, color='blue')
temp_graph.hlines(y=set_point, xmin=0, xmax=len(dicts), color='black')

heat_graph.plot(x, heat_y, color='green')

plt.show()
