#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

const char *netsetup_path = "./net-setup.sh";
const char *zflserver_conf = "zflserver.conf";
const char *zflserver_i = "zflserver.1";

int setup_net_interface(const char *conf, const char *command, const char *iname)
{
	pid_t child = fork();
	if (child < 0) {
		fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (child == 0) {
		int ret = execlp(netsetup_path,
				// "sudo",
				netsetup_path, "-c", conf, "-i", iname, command, NULL);

		if (ret < 0) {
			fprintf(stderr,
					"ERROR: could not run net-setup for %s %s as a child process: %s\n",
					iname, command, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	int status;
	wait(&status);

	return WEXITSTATUS(status);
}

int setup_bridging(char *command, char *iname)
{
	pid_t child = fork();
	if (child < 0) {
		fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	if (child == 0) {
		int ret = execlp(netsetup_path,
				// "sudo",
				netsetup_path, "-c", zflserver_conf, "-i", iname, "-i2",
				zflserver_i, command, NULL);
		if (ret < 0) {
			fprintf(stderr,
					"ERROR: could not run net-setup for %s %s as a child process: %s\n",
					iname, command, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	int status;
	wait(&status);

	return WEXITSTATUS(status);
}

int start_server(void)
{
	char *server_dir = "./zflserver";
	char build_dir[100];
	snprintf(build_dir, sizeof(build_dir), "%s/out", server_dir);

	pid_t child = fork();
	if (child < 0) {
		fprintf(stderr, "ERROR: could not fork server: %s\n", strerror(errno));
		return 1;
	}
	char qemu_instance[80];
	if (snprintf(qemu_instance, 80, "QEMU_INSTANCE=%d", 1) < 0) {
		fprintf(stderr, "ERROR: could not format QEMU_INSTANCE\n");
		exit(1);
	}

	if (child == 0) {
		int ret = execlp("make", "make", "run",

				"-C", build_dir,

				qemu_instance,

				"-j", "8", "-k",

				NULL);
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

int start_client(void)
{
	char *dir = "./zfl_client";
	char build_dir[100];
	snprintf(build_dir, sizeof(build_dir), "%s/out", dir);

	char qemu_instance[80];
	char zflclient_i[80];

	int instances = 1;
	for (int i = 0; i < instances; i++) {
		pid_t child = fork();
		if (child < 0) {
			fprintf(stderr, "ERROR: could not fork client: %s\n", strerror(errno));
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

		if (WEXITSTATUS(status) == EXIT_FAILURE) {
			return EXIT_FAILURE;
		}
		if (snprintf(zflclient_i, sizeof(zflclient_i), "zflclient.%d", i + 1) < 0) {
			fprintf(stderr, "ERROR: could not format zflclient_i\n");
			return EXIT_FAILURE;
		}
		if (setup_net_interface("zflclient.conf", "stop", zflclient_i) == -1) {
			fprintf(stderr, "ERROR: failed to net stop zflclient %s\n", zflclient_i);
			return EXIT_FAILURE;
		};
	}
	return 0;
}

int main(void)
{
	int clients = 1;
        // Setup the server networking
	if (setup_net_interface(zflserver_conf, "start", zflserver_i) == EXIT_FAILURE) {
		fprintf(stderr, "ERROR: failed to setup server conf\n");
		return -1;
	}
	char zflclient_i[80];
	for (int i = 0; i < clients; i++) {
		if (snprintf(zflclient_i, sizeof(zflclient_i), "zflclient.%d", i + 1) < 0) {
			fprintf(stderr, "ERROR: could not format zflclient_i\n");
			return 1;
		}
		if (setup_net_interface("zflclient.conf", "start", zflclient_i) == -1) {
			fprintf(stderr, "ERROR: failed to setup zflclient %s\n", zflclient_i);
			return 1;
		};
		if (setup_bridging("bridgeup", zflclient_i) == -1) {
			fprintf(stderr, "ERROR: failed to setup zflclient %s\n", zflclient_i);
			return 1;
		};
	}

	if (start_server() == EXIT_FAILURE) {
		fprintf(stderr, "ERROR: failed to setup server\n");
		goto shutdown;
	}
	printf("Waiting for server to start\n");
	sleep(5);
	// if (start_client() == EXIT_FAILURE) {
	// 	fprintf(stderr, "ERROR: failed to setup client\n");
	// 	goto shutdown;
	// }
	//
	int status;
	wait(&status);

	if (WEXITSTATUS(status) == EXIT_FAILURE) {
		fprintf(stderr, "ERROR: failed to setup server\n");
		goto shutdown;
	}
shutdown:
	if (setup_net_interface(zflserver_conf, "stop", zflserver_i) == EXIT_FAILURE) {
		fprintf(stderr, "ERROR: failed to stop server conf\n");
		return -1;
	}
	for (int i = 0; i < clients; i++) {
		if (snprintf(zflclient_i, sizeof(zflclient_i), "zflclient.%d", i + 1) < 0) {
			fprintf(stderr, "ERROR: could not format zflclient_i\n");
			return 1;
		}
		if (setup_net_interface("zflclient.conf", "stop", zflclient_i) == -1) {
			fprintf(stderr, "ERROR: failed to stop zflclient %s\n", zflclient_i);
			return 1;
		};
		if (setup_bridging("bridgedown", zflclient_i) == -1) {
			fprintf(stderr, "ERROR: failed to bridgedown %s\n", zflclient_i);
			return 1;
		};
	}
}
