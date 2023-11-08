# AIoT Emulator

Emulating federated machine learning using QEMU and Zephyr RTOS.

## Quick Setup

First, run `make` to build the `zfl` tool.

### Clients

```
// Build target
make client
// Start n clients
sudo ./zfl client <NUM_CLIENTS>
```

### Server

```
make server

sudo ./zfl server
```
