#include "sb.h"
#include "zflclient/src/sb.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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
        StringBuilder sb = {0};
        sb_init(&sb, 1024);
        if (conf != NULL) {
            sb_appendf(&sb, " -c %s", conf);
        }
        if (iname != NULL) {
            sb_appendf(&sb, " -i %s", iname);
        }
        if (command != NULL) {
            sb_appendf(&sb, " %s", command);
        }
        printf("Executing command: %s\n", sb.data);
        int ret = execlp(netsetup_path,
                         // "sudo",
                         netsetup_path, sb.data, NULL);

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
    char build_dir[100];
    snprintf(build_dir, sizeof(build_dir), "%s/out", server_dir);

    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "ERROR: could not fork server: %s\n", strerror(errno));
        return 1;
    }
    if (child == 0) {
        chdir("./zflserver");
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

int start_client(void) {
    char *dir = "./zflclient";
    char build_dir[100];
    snprintf(build_dir, sizeof(build_dir), "%s/out", dir);

    char qemu_instance[80];

    int instances = 1;
    for (int i = 0; i < instances; i++) {
        pid_t child = fork();
        if (child < 0) {
            fprintf(stderr, "ERROR: could not fork client: %s\n",
                    strerror(errno));
            return 1;
        }

        if (snprintf(qemu_instance, 80, "QEMU_INSTANCE=%d", i + 2) < 0) {
            fprintf(stderr, "ERROR: could not format QEMU_INSTANCE\n");
            return 1;
        }

        if (child == 0) {
            int ret = execlp("make", "make", "run",

                             "-C", build_dir,

                             qemu_instance,

                             "-j", "8", "-k", NULL);
            if (ret < 0) {
                fprintf(stderr,
                        "ERROR: could not run start client as a child "
                        "process: "
                        "%s\n",
                        strerror(errno));
                exit(1);
            }
        }
    }

    int status;
    for (int i = 0; i < instances; i++) {
        wait(&status);

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
    start_client();

    // server wait
    wait(NULL);

    if (setup_net_interface(NULL, "stop", NULL) == EXIT_FAILURE) {
        fprintf(stderr, "ERROR: failed to stop server conf\n");
        return -1;
    }
}
