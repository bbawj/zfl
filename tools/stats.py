import numpy as np
import matplotlib.pyplot as plt
import json

with open("../zflserver/stats.json") as json_file:
    json_data = json.load(json_file)
# Extracting relevant statistics
    training_time_avg = []
    sent_sum = [0]
    received_sum = [0]
    received_avg = []
    dropped_sum = []
    rexmit_sum = []

    for round_data in json_data:
        training_time_round = [client["training_time"] for client in round_data["stats"]]
        bytes_sent_round = [client["sent"] for client in round_data["stats"]]

        bytes_received_round = [client["recv"] for client in round_data["stats"]]
        dropped_round = [client["dropped"] for client in round_data["stats"]]
        rexmit_round = [client["rexmit"] for client in round_data["stats"]]

        training_time_avg.append(np.mean(training_time_round))
        sent_sum.append(np.sum(bytes_sent_round))
        received_sum.append(np.sum(bytes_received_round))
        dropped_sum.append(np.sum(dropped_round))
        rexmit_sum.append(np.sum(rexmit_round))

# Plotting
    rounds = range(1, len(json_data) + 1)
    actual_sent = [y - x for x, y in zip(sent_sum, sent_sum[1:])]
    actual_recv = [y - x for x, y in zip(received_sum, received_sum[1:])]

    plt.figure(figsize=(10, 5))

    # plt.plot(rounds, training_time_avg, label='Average Training Time', marker='o')
    plt.plot(rounds[1:], actual_sent[1:], label='Bytes Sent', marker='o')
    plt.plot(rounds[1:], actual_recv[1:], label='Bytes Received', marker='o')

    plt.xlabel('Round')
    plt.ylabel('Average')
    plt.title('Average Statistics Over 20 Rounds')
    plt.legend()
    plt.grid(True)
    plt.show()

