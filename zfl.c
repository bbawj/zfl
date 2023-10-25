#include "sb.h"
#include "zflclient/src/sb.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
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
const char *zflserver_conf = "zflserver.conf";
const char *zflserver_i = "zflserver.1";

int setup_net_interface(const char *conf, const char *command,
                        const char *iname) {
    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (child == 0) {
        int ret = execl(netsetup_path,
                        // "sudo",
                        netsetup_path,

                        command, NULL);

        if (ret < 0) {
            fprintf(stderr,
                    "ERROR: could not run net-setup for %s %s as a child "
                    "process: %s\n",
                    iname, command, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    int status;
    wait(&status);

    return WEXITSTATUS(status);
}

int setup_bridging(char *command, char *iname) {
    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    if (child == 0) {
        int ret = execlp(netsetup_path,
                         // "sudo",
                         netsetup_path, "-c", zflserver_conf, "-i", iname,
                         "-i2", zflserver_i, command, NULL);
        if (ret < 0) {
            fprintf(stderr,
                    "ERROR: could not run net-setup for %s %s as a child "
                    "process: %s\n",
                    iname, command, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    int status;
    wait(&status);

    return WEXITSTATUS(status);
}

int start_server(void) {
    char *server_dir = "./zflserver";

    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "ERROR: could not fork server: %s\n", strerror(errno));
        return 1;
    }
    if (child == 0) {
        chdir(server_dir);
        int ret = execlp("./server", "./server", NULL);
        if (ret < 0) {
            fprintf(stderr,
                    "ERROR: could not run server start as a child process: "
                    "%s\n",
                    strerror(errno));
            exit(1);
        }
    }

    return 0;
}

int start_client(int instances) {
    char *dir = "./zflclient";
    char build_dir[100];
    snprintf(build_dir, sizeof(build_dir), "%s/out", dir);

    char qemu_instance[80];
    char pipe_path[30];

    for (int i = 0; i < instances; i++) {
        if (snprintf(pipe_path, sizeof(pipe_path), "/tmp/client%d", i + 1) <
            0) {
            fprintf(stderr, "ERROR: could not format pipe path\n");
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

        pid_t child = fork();
        if (child < 0) {
            fprintf(stderr, "ERROR: could not fork client %d: %s\n", i,
                    strerror(errno));
            return 1;
        }

        // if (snprintf(qemu_instance, 80, "QEMU_INSTANCE=MAIN") < 0) {
        //     fprintf(stderr, "ERROR: could not format QEMU_INSTANCE\n");
        //     return 1;
        // }
        if (child == 0) {
            char *mac = malloc(100);
            srand(time(NULL) + getpid());

            snprintf(mac, 100, "%s", "00:");
            for (i = 0; i < 5; i++) {
                int tp = rand() % 32;
                char temp[10];
                snprintf(temp, sizeof(temp), "%s%x%s", tp < 16 ? "0" : "", tp,
                         i < 4 ? ":" : "\n");
                char temp_mac[50];
                snprintf(temp_mac, sizeof(temp_mac), "%s%s", mac, temp);
                mac = strdup(temp_mac);
            }
            printf("Client %d: generated MAC: %s\n", i, mac);
            // /opt/zephyr-sdk-0.16.1/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-i386
            // -m 50 -cpu qemu32,+nx,+pae -machine q35 -device
            // isa-debug-exit,iobase=0xf4,iosize=0x04 -no-reboot -nographic
            // -no-acpi -nic tap,model=e1000,script=no,downscript=no,ifname=zeth
            // -pidfile qemu${QEMU_INSTANCE}.pid -chardev stdio,id=con,mux=on
            // -serial chardev:con -mon chardev=con,mode=readline -kernel
            // /home/bawj/projects/fyp/zephyrproject/zephyr/zfl/zflclient/out/zephyr/zephyr.elf
            char serial_arg[80];
            snprintf(serial_arg, 80, "pipe:%s", pipe_path);

            char nic_arg[100];
            snprintf(nic_arg, sizeof(nic_arg),
                     "bridge,model=e1000,mac=00:00:5e:00:53:ff,br=zfl");

            int ret =
                execlp("/opt/zephyr-sdk-0.16.1/sysroots/"
                       "x86_64-pokysdk-linux/usr/bin/qemu-system-i386",
                       "/opt/zephyr-sdk-0.16.1/sysroots/"
                       "x86_64-pokysdk-linux/usr/bin/qemu-system-i386",

                       "-m", "50", "-cpu", "qemu32,+nx,+pae", "-machine", "q35",
                       "-device", "isa-debug-exit,iobase=0xf4,iosize=0x04",

                       "-no-reboot", "-nographic", "-no-acpi",
                       // "-pidfile", "qemu${QEMU_INSTANCE}.pid",
                       // "-chardev", serial_arg,
                       "-chardev", "stdio,id=con",

                       "-serial", serial_arg,

                       "-mon", "chardev=con,mode=readline",

                       "-nic", nic_arg,

                       // "-serial", "con",

                       "-kernel",
                       "/home/bawj/projects/fyp/zephyrproject/zephyr/zfl/"
                       "zflclient/out/zephyr/zephyr.elf",

                       // "-j", "8", "-k",

                       NULL);
            if (ret < 0) {
                fprintf(stderr,
                        "ERROR: could not run start client as a child "
                        "process: "
                        "%s\n",
                        strerror(errno));
                exit(1);
            }
        }

        int in = open(pipe_in, O_WRONLY);
        if (in < 0) {
            perror("open pipe");
            return -1;
        }
        char run_cmd[50];
        snprintf(run_cmd, sizeof(run_cmd), "run 192.0.2.%d\n", i + 3);
        printf("INFO: sending command '%s' to %s\n", run_cmd, pipe_in);
        if (write(in, run_cmd, strlen(run_cmd) + 1) < 0) {
            printf("write failed IP %s to FIFO pipe %s", run_cmd, pipe_path);
            return -1;
        }
        close(in);
    }

    int status;
    for (int i = 0; i < instances; i++) {
        wait(&status);
        snprintf(pipe_path, sizeof(pipe_path), "/tmp/client%d", i + 1);

        char pipe_in[100], pipe_out[100];
        snprintf(pipe_in, sizeof(pipe_in), "%s.in", pipe_path);
        snprintf(pipe_out, sizeof(pipe_out), "%s.out", pipe_path);
        remove(pipe_in);
        remove(pipe_out);

        if (WEXITSTATUS(status) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return 0;
}

int main(void) {
    // int clients = 1;
    // Setup the server networking
    if (setup_net_interface(NULL, "start", NULL) == EXIT_FAILURE) {
        fprintf(stderr, "ERROR: failed to setup server conf\n");
        return -1;
    }

    start_server();
    start_client(1);

    // server wait
    wait(NULL);
    wait(NULL);
    //
    // if (setup_net_interface(NULL, "stop", NULL) == EXIT_FAILURE) {
    //     fprintf(stderr, "ERROR: failed to stop server conf\n");
    //     return -1;
    // }
}
