#!/bin/bash

export DV_SIMULATORS=veri-testharness,spike
source verif/sim/setup-env.sh
cd verif/sim
python3 cva6.py \
  --c_tests ../tests/custom/hello_world/hello_world.c \
  --iss_yaml cva6.yaml \
  --target cv64a6_imafdc_sv39 \
  --iss=$DV_SIMULATORS \
  --gcc_opts="-static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -g ../tests/custom/common/syscalls.c ../tests/custom/common/crt.S -I../tests/custom/env -I../tests/custom/common -lgcc -nostdlib -lgcc" \
  --linker=../../config/gen_from_riscv_config/linker/link.ld \
  --issrun_opts=+debug_disable=1+UVM_VERBOSITY=UVM_NONE

python3 cva6.py \
  --c_tests ../tests/custom/hello_world/hello_world.c \
  --iss_yaml cva6.yaml \
  --target cv64a6_imafdc_mojov_sv39 \
  --iss=$DV_SIMULATORS \
  --gcc_opts="-static -mcmodel=medany -fvisibility=hidden -nostdlib -nostartfiles -g ../tests/custom/common/syscalls.c ../tests/custom/common/crt.S -I../tests/custom/env -I../tests/custom/common -lgcc -nostdlib -lgcc" \
  --linker=../../config/gen_from_riscv_config/linker/link.ld \
  --issrun_opts=+debug_disable=1+UVM_VERBOSITY=UVM_NONE
