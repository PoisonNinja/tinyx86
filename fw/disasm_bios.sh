#!/bin/bash

ndisasm -b 16 -o0xE0000 -a ./bios.bin | less
