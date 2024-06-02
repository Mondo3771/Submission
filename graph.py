import matplotlib.pyplot as plt

# Number of threads
threads = [2, 4, 8, 16, 32, 64, 128, 256]

# Corresponding speedup values
speedup = [0.316548, 0.496615, 0.669296, 0.775642, 0.781515, 0.777994, 0.778264, 0.776051]

# Corresponding time taken values
time_taken = [11.382653, 7.219549, 5.419865, 4.649495, 4.599116, 4.619242, 4.630196, 4.660613]

fig, axs = plt.subplots(2)
fig.suptitle('Bitonic Sort with OpenMP')

axs[0].plot(threads, speedup, marker='o')
axs[0].set(xlabel='Number of Threads', ylabel='Speedup')

axs[1].plot(threads, time_taken, marker='o')
axs[1].set(xlabel='Number of Threads', ylabel='Time Taken')

plt.show()