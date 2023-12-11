#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

const char *netsetup_path = "./net-setup.sh";

int setup_net_interface(const char *conf, const char *command,
                        const char *iname) {
    pid_t child = fork();
    if (child < 0) {
        printf("ERROR: could not fork a child: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (child == 0) {
        int ret = execl(netsetup_path,
                        // "sudo",
                        netsetup_path,

                        command, NULL);

        if (ret < 0) {
            printf("ERROR: could not run net-setup for %s %s as a child "
                   "process: %s\n",
                   iname, command, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    int status;
    wait(&status);

    return WEXITSTATUS(status);
}

int start_server(int num_rounds, int clients_per_round) {
    char *server_dir = "./zflserver";

    pid_t child = fork();
    if (child < 0) {
        printf("ERROR: could not fork server: %s\n", strerror(errno));
        return 1;
    }
    printf("INFO: launching server process pid: %d\n", child);
    if (child == 0) {
        chdir(server_dir);
        char nr[100];
        snprintf(nr, sizeof(nr), "%d", num_rounds);
        char cpr[100];
        snprintf(cpr, sizeof(cpr), "%d", clients_per_round);
        int ret = execlp("./server", "./server", nr, cpr, NULL);
        if (ret < 0) {
            printf("ERROR: could not run server start as a child process: "
                   "%s\n",
                   strerror(errno));
            exit(1);
        }
    }

    return 0;
}

void *cat_client_output(void *pipe_path) {
    char *path = (char *)pipe_path;
    FILE *log = NULL;
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: Could not open file %s because %s\n", path,
               strerror(errno));
        goto clean;
    }

    char log_path[100];
    snprintf(log_path, sizeof(log_path), "%s.log", path);
    log = fopen(log_path, "w");
    if (log == NULL) {
        printf("ERROR: Could not open log file %s because %s\n", log_path,
               strerror(errno));
        goto clean;
    }
    char buf[1024];
    int n_read = read(fd, buf, sizeof(buf));
    while (n_read > 0) {
        if (fwrite(buf, n_read, 1, log) <= 0) {
            printf("ERROR: failed to write to log file %s because %s\n",
                   log_path, strerror(errno));
            goto clean;
        }
        fflush(log);
        n_read = read(fd, buf, sizeof(buf));
    }
clean:
    free(pipe_path);
    close(fd);
    fclose(log);
    return NULL;
}

int start_client(int num_clients, size_t epochs, size_t batch_size) {
    char *dir = "./zflclient";
    char build_dir[100];
    snprintf(build_dir, sizeof(build_dir), "%s/out", dir);

    char pipe_path[30];
    pid_t process_ids[num_clients];

    for (int i = 0; i < num_clients; i++) {
        if (snprintf(pipe_path, sizeof(pipe_path), "/tmp/client%d", i + 1) <
            0) {
            printf("ERROR: could not format pipe path\n");
            return 1;
        }

        char pipe_in[100], pipe_out[100];
        snprintf(pipe_in, sizeof(pipe_in), "%s.in", pipe_path);
        snprintf(pipe_out, sizeof(pipe_out), "%s.out", pipe_path);
        if (mkfifo(pipe_in, 0666) == -1) {
            printf("ERROR: mkfifo %s failed because: %s", pipe_in,
                   strerror(errno));
            return -1;
        }
        if (mkfifo(pipe_out, 0666) == -1) {
            printf("ERROR: mkfifo %s failed because: %s", pipe_out,
                   strerror(errno));
            return -1;
        }

        char *mac = malloc(100);
        srand(time(NULL) + getpid());

        snprintf(mac, 100, "%s", "00:");
        for (int x = 0; x < 5; x++) {
            int tp = rand() % 32;
            char temp[10];
            snprintf(temp, sizeof(temp), "%s%x%s", tp < 16 ? "0" : "", tp,
                     x < 4 ? ":" : "");
            char temp_mac[50];
            snprintf(temp_mac, sizeof(temp_mac), "%s%s", mac, temp);
            free(mac);
            mac = strdup(temp_mac);
        }
        printf("Client %d: generated MAC: %s\n", i, mac);

        char serial_arg[80];
        snprintf(serial_arg, 80, "pipe:%s", pipe_path);

        char nic_arg[100];
        snprintf(nic_arg, sizeof(nic_arg), "bridge,model=e1000,mac=%s,br=zfl",
                 mac);

        pid_t child = fork();
        if (child < 0) {
            printf("ERROR: could not fork client %d: %s\n", i, strerror(errno));
            return 1;
        }
        if (child != 0) {
            process_ids[i] = child;
        } else {
            int ret =
                execlp("qemu-system-i386", "qemu-system-i386",

                       "-m", "15", "-cpu", "qemu32,+nx,+pae", "-machine", "q35",
                       "-device", "isa-debug-exit,iobase=0xf4,iosize=0x04",

                       "-no-reboot", "-nographic", "-no-acpi",

                       "-serial", serial_arg,

                       "-nic", nic_arg,

                       "-kernel", "./zflclient/out/zephyr/zephyr.elf",

                       NULL);
            if (ret < 0) {
                printf("ERROR: could not run start client as a child "
                       "process: "
                       "%s\n",
                       strerror(errno));
                exit(1);
            }
        }

        sleep(3);

        int in = open(pipe_in, O_WRONLY);
        if (in < 0) {
            printf("ERROR: could not open pipe %s\n", pipe_in);
            return -1;
        }
        char run_cmd[50];
        snprintf(run_cmd, sizeof(run_cmd), "run 192.0.2.%d %zu %zu\n", i + 3,
                 epochs, batch_size);
        printf("INFO: sending command '%s' to %s\n", run_cmd, pipe_in);
        if (write(in, run_cmd, strlen(run_cmd) + 1) < 0) {
            printf("write failed IP %s to FIFO pipe %s\n", run_cmd, pipe_path);
            return -1;
        }
        close(in);

        pthread_t th;
        pthread_create(&th, NULL, cat_client_output, strdup(pipe_out));
    }

    for (int i = 0; i < num_clients; i++) {
        int pid = wait(NULL);
        printf("client %d exited\n", pid);
        for (int j = 0; j < num_clients; ++j) {
            if (process_ids[j] == pid) {
                snprintf(pipe_path, sizeof(pipe_path), "/tmp/client%d", j + 1);

                char pipe_in[100], pipe_out[100];
                snprintf(pipe_in, sizeof(pipe_in), "%s.in", pipe_path);
                snprintf(pipe_out, sizeof(pipe_out), "%s.out", pipe_path);
                remove(pipe_in);
                remove(pipe_out);
            }
        }
    }
    return 0;
}

void usage() {
    printf(
        "Usage:\n./zfl [server|client] [OPTIONS]\n\nOPTIONS:\n  server:\n    "
        "num_rounds\n    clients_per_round\n  client:\n    num_clients\n    "
        "epochs\n    batch_size");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage();
        return -1;
    }
    char *command = argv[1];
    if (strncmp(command, "server", strlen("server")) == 0) {
        int num_rounds = atoi(argv[2]);
        if (num_rounds == 0) {
            usage();
            return -1;
        }
        int clients_per_round = atoi(argv[3]);
        if (clients_per_round == 0) {
            usage();
            return -1;
        }
        // Setup the server networking
        if (setup_net_interface(NULL, "start", NULL) == EXIT_FAILURE) {
            printf("ERROR: failed to setup server conf\n");
            return -1;
        }

        start_server(num_rounds, clients_per_round);
        // server wait
        wait(NULL);

        if (setup_net_interface(NULL, "stop", NULL) == EXIT_FAILURE) {
            printf("ERROR: failed to stop server conf\n");
            return -1;
        }
    } else if (strncmp(command, "client", strlen("client")) == 0) {
        int num_clients = atoi(argv[2]);
        if (num_clients == 0) {
            usage();
            return -1;
        }
        int epochs = atoi(argv[3]);
        if (epochs == 0) {
            usage();
            return -1;
        }
        int batch_size = atoi(argv[4]);
        if (batch_size == 0) {
            usage();
            return -1;
        }
        start_client(num_clients, epochs, batch_size);
    } else {
        usage();
        return -1;
    }
}
