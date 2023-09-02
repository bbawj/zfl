#include "asm-generic/ioctl.h"
#include <stdio.h>
#include <zephyr/net/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <zephyr/posix/sys/ioctl.h>
#include <unistd.h>

#define TUNDEV        "/dev/net/tun"
#define TUNSETIFF     _IOW('T', 202, int)
#define TUNSETPERSIST _IOW('T', 203, int)
#define TUNSETOWNER   _IOW('T', 204, int)
#define TUNSETGROUP   _IOW('T', 206, int)

/*
    echo "Creating $IFACE"
    ip tuntap add $IFACE mode tap $@

    # The idea is that the configuration file will setup
    # the IP addresses etc. for the created interface.
    . "$CONF_FILE" $IFACE
    */
static int tap_add_ioctl(struct ifreq *ifr, uid_t uid, gid_t gid)
{
	int fd;
	int ret = -1;

#ifdef IFF_TUN_EXCL
	ifr->ifr_flags |= IFF_TUN_EXCL;
#endif

	fd = open(TUNDEV, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	if (ioctl(fd, TUNSETIFF, ifr)) {
		perror("ioctl(TUNSETIFF)");
		goto out;
	}
	if (uid != -1 && ioctl(fd, TUNSETOWNER, uid)) {
		perror("ioctl(TUNSETOWNER)");
		goto out;
	}
	if (gid != -1 && ioctl(fd, TUNSETGROUP, gid)) {
		perror("ioctl(TUNSETGROUP)");
		goto out;
	}
	if (ioctl(fd, TUNSETPERSIST, 1)) {
		perror("ioctl(TUNSETPERSIST)");
		goto out;
	}
	ret = 0;
out:
	close(fd);
	return ret;
}

/*
    ip link set $IFACE down

    if [ -f "$CONF_FILE".stop ]; then
	. "$CONF_FILE".stop $IFACE
    fi

    echo "Removing $IFACE"
    ip tuntap del $IFACE mode tap
 */
static int tap_del_ioctl(struct ifreq *ifr)
{
	int fd = open(TUNDEV, O_RDWR);
	int ret = -1;

	if (fd < 0) {
		perror("open");
		return -1;
	}
	if (ioctl(fd, TUNSETIFF, ifr)) {
		perror("ioctl(TUNSETIFF)");
		goto out;
	}
	if (ioctl(fd, TUNSETPERSIST, 0)) {
		perror("ioctl(TUNSETPERSIST)");
		goto out;
	}
	ret = 0;
out:
	close(fd);
	return ret;
}
