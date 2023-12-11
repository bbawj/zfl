# zfl

Emulating federated machine learning using QEMU and Zephyr RTOS.

## Getting Started

### Dependencies

#### ZephyrOS
First, follow the Zephyr OS [setup guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) to install `west` and the Zephyr SDK.

From within your copy of the Zephyr source code repository, clone `zfl` as a [repository application](https://docs.zephyrproject.org/latest/develop/application/index.html#zephyr-repository-application). This ensures that Zephyr kernel dependencies are accessible for the client build.

Run `make` to bootstrap the main `zfl` tool.

#### QEMU
zfl uses `qemu-system-i386` by default and searches for an executable with that name on your path.

### Clients

#### Configuration

The standard configuration for the QEMU board provided by zephyr can be tweaked to allocate more SRAM. This is especially needed for machine learning.

1. Locate the qemu_x86 board file somewhere in `<zephyr_root>/boards/x86/qemu_x86/qemu_x86.dts`
2. In the standard `zflclient` 15MB RAM is used. Configure this needed memory with: `#define DT_DRAM_SIZE DT_SIZE_K(15360)`
3. Additional (or less) memory can also be configured for `zflclient` by changing these parameters in `zflclient/prj.conf`:

```
CONFIG_SRAM_SIZE=15360
CONFIG_MAIN_STACK_SIZE=8192
CONFIG_KERNEL_VM_SIZE=0x7000000
CONFIG_HEAP_MEM_POOL_SIZE=10485760
```

#### Building

With `zfl` built, run the following commands from the `zfl` root directory to start clients running in QEMU x86.

```
// Build target
make client
// Start n clients
sudo ./zfl client <num_clients> <epochs> <batch_size>
```

### Server

With `zfl` built, run the following commands from the `zfl` root directory to start the central server.

```
make server

sudo ./zfl server <num_rounds> <clients_per_round>
```

## Why

Federated learning allows a fleet of devices to collaborate towards a globally trained machine
learning model. Research has continued to produce novel federated learning algorithms to tackle
different issues in FL such as heteorogenity and learning over data from non-identical
distributions. Performance of these algorithms depend in part on the system parameters used in FL
such as number of clients and number of passes. Furthermore, a realistic benchmark would require
one to procure a large fleet of devices. This work seeks to introduce a software emulation framework to
streamline the process of building a fleet of clients and configuring system parameters.

## Credits

The server code comes baked in with `nn.h` by Tsoding.
