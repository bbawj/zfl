# zfl

Emulating federated machine learning using QEMU and Zephyr RTOS.

## Getting Started

### Dependencies

First, follow the Zephyr OS [setup guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) to install `west` and the Zephyr SDK.

From within your copy of the Zephyr source code repository, clone `zfl` as a [repository application](https://docs.zephyrproject.org/latest/develop/application/index.html#zephyr-repository-application). This ensures that Zephyr kernel dependencies are accessible for the client build.

Run `make` to build the main `zfl` tool.

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

