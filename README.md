git clone https://github.com/satish-pati/3_Compilers_LabProject.git

cd 3_Compilers_LabProject/compiler

#### Build the compiler
make

#### compiler
sudo apt install graphviz

chmod +x compiler

./compiler ../demo/temp

./compiler ../cases_asm/c1
 
sudo apt install gcc-riscv64-linux-gnu qemu-user

#### Generate assembly from TAC
python3 tac_to_risc.py opt.tac output.s

#### Compile assembly to executable
riscv64-linux-gnu-gcc -static -o program output.s

#### Run on emulator
qemu-riscv64 program

#### All in one (with --run flag)
python3 tac_to_risc.py opt.tac output.s --run
