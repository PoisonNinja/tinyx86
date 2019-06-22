#!/bin/bash

ndisasm -b 16 -o0xC0000 -a ./bios.bin | less
