import numpy as np
import matplotlib.pyplot as plt
import json
import time

with open("stats-loss.json") as json_file_loss:
    with open("stats.json") as json_file_normal:
        json_data_normal = json.load(json_file_normal)
        json_data_loss = json.load(json_file_loss)
# Extracting relevant statistics
        loss = []
        accuracy = []

        normal_training_time_avg = []
        normal_sent_sum = []
        normal_received_sum = []
        normal_received_avg = []
        normal_dropped_sum = []
        normal_rexmit_sum = []

        loss_training_time_avg = []
        loss_sent_sum = []
        loss_received_sum = []
        loss_received_avg = []
        loss_dropped_sum = []
        loss_rexmit_sum = []

        for round_data in json_data_normal:
            training_time_round = [client["training_time"] for client in round_data["stats"]]
            bytes_sent_round = [client["sent"] for client in round_data["stats"]]

            bytes_received_round = [client["recv"] for client in round_data["stats"]]
            dropped_round = [client["dropped"] for client in round_data["stats"]]
            rexmit_round = [client["rexmit"] for client in round_data["stats"]]

            accuracy.append(round_data["accuracy"])
            loss.append(round_data["loss"])
            normal_training_time_avg.append(np.mean(training_time_round))
            normal_sent_sum.append(np.sum(bytes_sent_round))
            normal_received_sum.append(np.sum(bytes_received_round))
            normal_dropped_sum.append(np.sum(dropped_round))
            normal_rexmit_sum.append(np.sum(rexmit_round))

        for round_data in json_data_loss:
            training_time_round = [client["training_time"] for client in round_data["stats"]]
            bytes_sent_round = [client["sent"] for client in round_data["stats"]]

            bytes_received_round = [client["recv"] for client in round_data["stats"]]
            dropped_round = [client["dropped"] for client in round_data["stats"]]
            rexmit_round = [client["rexmit"] for client in round_data["stats"]]

            loss_training_time_avg.append(np.mean(training_time_round))
            loss_sent_sum.append(np.sum(bytes_sent_round))
            loss_received_sum.append(np.sum(bytes_received_round))
            loss_dropped_sum.append(np.sum(dropped_round))
            loss_rexmit_sum.append(np.sum(rexmit_round))

# Plotting
        rounds = range(1, len(json_data_loss) + 1)
        loss_actual_sent = [max(y - x, 0) for x, y in zip(loss_sent_sum, loss_sent_sum[1:])]
        loss_actual_recv = [max(y - x, 0) for x, y in zip(loss_received_sum, loss_received_sum[1:])]
        loss_actual_dropped = [max(y - x, 0) for x, y in zip(loss_dropped_sum, loss_dropped_sum[1:])]
        loss_actual_rexmit = [max(y - x,0) for x, y in zip(loss_rexmit_sum, loss_rexmit_sum[1:])]

        normal_actual_sent = [max(y - x, 0) for x, y in zip(normal_sent_sum, normal_sent_sum[1:])]
        normal_actual_recv = [max(y - x, 0) for x, y in zip(normal_received_sum, normal_received_sum[1:])]
        normal_actual_dropped = [max(y - x, 0) for x, y in zip(normal_dropped_sum, normal_dropped_sum[1:])]
        normal_actual_rexmit = [max(y - x, 0) for x, y in zip(normal_rexmit_sum, normal_rexmit_sum[1:])]

        plt.figure(figsize=(10, 5))
        plt.xticks(rounds, rounds)

        # plt.plot(rounds, training_time_avg, label='Average Training Time', marker='o')

        plt.xticks(rounds, rounds)
        plt.xlabel('Round')
        plt.ylabel('Time (s)')
        plt.plot(rounds[0:], normal_training_time_avg[0:], label='Training Time', marker='o')
        plt.legend()
        plt.savefig(f"avg-training.eps", format="eps")
        plt.close()

        plt.xticks(rounds, rounds)
        plt.xlabel('Round')
        plt.ylabel('Loss')
        plt.plot(rounds[0:], loss[0:], label='Loss', marker='o')
        plt.savefig(f"loss.eps", format="eps")
        plt.close()

        plt.xticks(rounds, rounds)
        plt.xlabel('Round')
        plt.ylabel('Accuracy')
        plt.plot(rounds[0:], accuracy[0:], label='Accuracy', marker='o')
        plt.savefig(f"accuracy.eps", format="eps")
        plt.close()

        plt.ylabel('Bytes')
        plt.xticks(rounds, rounds)
        plt.title('Bytes Sent per Round')
        plt.plot(rounds[1:], loss_actual_sent[0:], label='Normal', marker='o')
        plt.plot(rounds[1:], normal_actual_sent[0:], label='10% Loss', marker='o')
        plt.legend()
        plt.savefig(f"bytes-sent.eps", format="eps")
        plt.close()

        plt.ylabel('Bytes')
        plt.xticks(rounds, rounds)
        plt.title('Bytes Received per Round')
        plt.plot(rounds[1:], loss_actual_recv[0:], label='10% Loss', marker='o')
        plt.plot(rounds[1:], normal_actual_recv[0:], label='Normal', marker='o')
        plt.legend()
        plt.savefig(f"bytes-recv.eps", format="eps")
        plt.close()

        plt.ylabel('Segments')
        plt.xticks(rounds, rounds)
        plt.title('TCP Segments Dropped per Round')
        plt.plot(rounds[0:19], loss_actual_dropped[0:19], label='10% Loss', marker='o')
        plt.plot(rounds[0:19], normal_actual_dropped[0:19], label='Normal', marker='o')
        plt.legend()
        plt.savefig(f"tcp-dropped.eps", format="eps")
        plt.close()

        plt.ylabel('Segments')
        plt.xticks(rounds, rounds)
        plt.title('TCP Segments Retransmitted per Round')
        plt.plot(rounds[0:19], loss_actual_rexmit[0:19], label='10% Loss', marker='o')
        plt.plot(rounds[0:19], normal_actual_rexmit[0:19], label='Normal', marker='o')
        plt.legend()
        plt.savefig(f"tcp-rexmit.eps", format="eps")
        plt.close()

        # plt.title('Statistics Over 20 Rounds')
        # plt.show()

