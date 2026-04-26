-- west build: running target run
[0/1] To exit from QEMU enter: 'CTRL+a...,c=on,zicsr=on,zifencei=on,pmp=on,u=on
qemu-system-riscv32: -m 256-cpu: Parameter 'size' expects a non-negative number below 2^64
Optional suffix k, M, G, T, P or E means kilo-, mega-, giga-, tera-, peta-
and exabytes, respectively.
FAILED: zephyr/CMakeFiles/run_qemu /disk/workspace/rvv/zephyr_app/build/zephyr/CMakeFiles/run_qemu
cd /disk/workspace/rvv/zephyr_app/build && /home/tome/zephyr-sdk-1.0.1-old/hosttools/sysroots/x86_64-pokysdk-linux/usr/bin/qemu-system-riscv32 -machine virt -bios none -m 256 -cpu rv32i,i=on,m=on,a=on,f=on,d=on,c=on,zicsr=on,zifencei=on,pmp=on,u=on -net none -pidfile qemu.pid -chardev stdio,id=con,mux=on -serial chardev:con -mon chardev=con,mode=readline -nographic -icount shift=6,align=off,sleep=off -rtc clock=vm -cpu rv32,v=true,vlen=128,vext_spec=v1.0 -machine virt -bios none -m 256-cpu max,len=64 -nographic -kernel /disk/workspace/rvv/zephyr_app/build/zephyr/zephyr.elf
ninja: build stopped: subcommand failed.
FATAL ERROR: command exited with status 1: /usr/bin/cmake --build /disk/workspace/rvv/zephyr_app/build --target run
