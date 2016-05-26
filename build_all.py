#!/usr/bin/python3

import os
from subprocess import Popen

TOPDIR = os.path.abspath(os.path.dirname(__file__))

# The list of configurations that will be built. Delete any from this list that
# you're not interested in.
configurations = [ 'x86_64gcc', 'x86_64clang',
                   'riscv32gcc', 'riscv32clang',
                   'riscv64gcc', 'riscv64clang',
                   'armv7mgcc', 'armv8mgcc', 'armv8agcc' ]


# If any folders need prepending to the path for a given configuration, then
# edit the appropriate value in here. If no additional paths need prepending
# for a given configuration, use the empty string ''.
toolchain_paths = {
    # x86_64
    'x86_64gcc': '',
    'x86_64clang': '/home/graham/projects/riscv-eval/install-llvm-master/bin',

    # RISC-V 32
    'riscv32gcc': '/home/graham/projects/riscv-eval/install-master-32/bin',
    'riscv32clang': '/home/graham/projects/riscv-eval/install-master/bin:' \
                    '/home/graham/projects/riscv-eval/install-master-32/bin',

    # RISC-V 64
    'riscv64gcc': '/home/graham/projects/riscv-eval/install-master/bin',
    'riscv64clang': '/home/graham/projects/riscv-eval/install-master/bin',

    # ARM
    'armv7mgcc': '/home/graham/projects/riscv-eval/gcc-arm-none-eabi-5_3-2016q1/bin',
    'armv8mgcc': '/home/graham/projects/riscv-eval/gcc-arm-none-eabi-5_3-2016q1/bin',
    'armv8agcc': '/home/graham/projects/riscv-eval/gcc-arm-none-eabi-5_3-2016q1/bin',
}


# The compiler flags for each configuration are given here. Some compilers
# require more flags than others in order to successfully compile for their
# target, but all us `-Os` in order to generate code optimised for size.
toolchain_args = {
    # x86_64 - no extra args required, as it is a native build.
    'x86_64gcc': '-Os',
    'x86_64clang': '-Os',

    # RISC-V 32
    # -m32 -march=RV32IC to generate code for RISC-V 32 base integer
    # architecture with the compressed instruction extension
    'riscv32gcc': '-Os -m32 -mrvc -march=RV32IC',
    # No -mrvc flag for Clang (may not be implemented at present). Additional
    # flags to allow compile / link in one step as described in the blog post.
    'riscv32clang': '-Os -target riscv-unknown-elf -mriscv=RV32IC -nostdinc ' \
                    '-isystem /home/graham/projects/riscv-eval/install-master-32/riscv32-unknown-elf/include ' \
                    '-isystem /home/graham/projects/riscv-eval/install-master/lib/clang/3.8.0/include',

    # RISC-V 64
    'riscv64gcc': '-Os',
    # Similar additional Clang flags as for RISC-V 32
    'riscv64clang': '-Os -target riscv64-unknown-elf -mriscv=RV64IAMFD -nostdinc ' \
                    '-isystem /home/graham/projects/riscv-eval/install-master/riscv64-unknown-elf/include ' \
                    '-isystem /home/graham/projects/riscv-eval/install-master/lib/clang/3.8.0/include',

    # ARM
    # -specs=nosys.specs in order to avoid link errors as recommended in the
    # FAQ: https://answers.launchpad.net/gcc-arm-embedded/+faq/2519
    # -mthumb for ARMv7m and ARMv8m in order to generate Thumb instructions
    # where possible.
    'armv7mgcc': '-Os -specs=nosys.specs -march=armv7-m -mthumb',
    'armv8mgcc': '-Os -specs=nosys.specs -march=armv8-m.base -mthumb',
    'armv8agcc': '-Os -specs=nosys.specs -march=armv8-a',
}


# The name of the compiler for each configuration.
toolchain_cc = {
    'x86_64gcc': 'gcc',
    'x86_64clang': 'clang',
    'riscv32gcc': 'riscv32-unknown-elf-gcc',
    'riscv32clang': 'clang',
    'riscv64gcc': 'riscv64-unknown-elf-gcc',
    'riscv64clang': 'clang',
    'armv7mgcc': 'arm-none-eabi-gcc',
    'armv8mgcc': 'arm-none-eabi-gcc',
    'armv8agcc': 'arm-none-eabi-gcc',
}


def make_env(config):
    e = os.environ.copy()
    e['PATH'] = '%s:%s' % (toolchain_paths[config], os.environ['PATH'])
    e['TGT'] = config
    e['OPT'] = toolchain_args[config]
    e['CC'] = toolchain_cc[config]

    return e


for config in configurations:
    env = make_env(config)
    args = ['make']
    proc = Popen(args, env=env, cwd=TOPDIR)
    retcode = proc.wait()
    if retcode != 0:
        raise RuntimeError("Make for %s failed with retcode %s" % (arch, retcode))
