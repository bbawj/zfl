#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	char *sample_dir = "./zflserver";
	char build_dir[100];
	snprintf(build_dir, sizeof(build_dir), "%s/out", sample_dir);
	pid_t child;
	// Setup the server networking
	child = fork();
	if (child < 0) {
		fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
		return -1;
	}
	if (child == 0) {
		int ret =
			execlp("/home/bawj/projects/fyp/zephyrproject/tools/net-tools/net-setup.sh",
			       "sudo",
			       "/home/bawj/projects/fyp/zephyrproject/tools/net-tools/net-setup.sh",
			       "-c", "zflserver.conf", "-i", "zflserver.1", "start", NULL);
		if (ret < 0) {
			fprintf(stderr, "ERROR: could not run net-setup as a child process: %s\n",
				strerror(errno));
			return -1;
		}
	}

	wait(NULL);

	child = fork();
	if (child < 0) {
		fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
		return -1;
	}
	if (child == 0) {
		int ret =
			execlp("/home/bawj/projects/fyp/zephyrproject/tools/net-tools/net-setup.sh",
			       "sudo",
			       "/home/bawj/projects/fyp/zephyrproject/tools/net-tools/net-setup.sh",
			       "-c", "zflserver.conf", "-i", "zflserver.1", "stop", NULL);
		if (ret < 0) {
			fprintf(stderr, "ERROR: could not run net-setup as a child process: %s\n",
				strerror(errno));
			return -1;
		}
	}

	wait(NULL);
	return 1;

	int instances = 1;
	for (int i = 0; i < instances; i++) {
		pid_t child = fork();
		if (child < 0) {
			fprintf(stderr, "ERROR: could not fork a child: %s\n", strerror(errno));
			return -1;
		}
		char qemu_instance[80];
		if (snprintf(qemu_instance, 80, "QEMU_INSTANCE=%d", i + 1) < 0) {
			fprintf(stderr, "ERROR: could not format QEMU_INSTANCE\n");
			return -1;
		}

		if (child == 0) {
			int ret = execlp("make", "make", "run",

					 // "--cmake-only",
					 "-C", build_dir,

					 // "--build-dir", build_dir,

					 // "-b", "qemu_x86",

					 // "-o", qemu_instance,
					 qemu_instance,
					 // "-o='--jobs=8'",
					 "-j", "8",

					 // "-t", "run", sample_dir,

					 // "--", "-G", "Unix Makefiles",

					 NULL);
			if (ret < 0) {
				fprintf(stderr,
					"ERROR: could not run west as a child process: %s\n",
					strerror(errno));
				return -1;
			}
		}
	}

	wait(NULL);
	// wait(NULL);
}
