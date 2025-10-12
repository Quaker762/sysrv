#
# System-RV QEMU run script
#
# Copyright (C) 2025 Jesse Buhagiar
#

from argparse import ArgumentParser
from os import environ as env
from os import getcwd as cwd
from sys import stdout, stdin
import subprocess


RUN_TYPES = ['launch', 'start_target']
SYSRV_DEFAULT_ARCH = 'riscv64'

QEMU_FLAGS = [
    '-machine',
    'virt',
    '-serial',
    'mon:stdio',
    '-S',
    '-s'
]


def _run_shell_cmd(command: str,
                   command_args: list[str]) -> bool:
    proc_args = [command]
    proc_args.extend(command_args)
    print("QEMU started")
    with subprocess.Popen(proc_args,
                          stdout=stdout,
                          stdin=stdin,
                          stderr=subprocess.PIPE,
                          cwd=env['SYSV_ROOT'],
                          text=True) as proc:
        try:
            output, errs = proc.communicate()

            if errs is not None:
                print(errs, flush=True)
        except Exception:
            proc.kill()


def parse_arguments() -> ArgumentParser:
    args_parser = ArgumentParser('System-RV Build Script')

    args_parser.add_argument('run_type',
                             choices=RUN_TYPES,
                             help='Possible run types')
    args_parser.add_argument('kernel_path',
                             help='Path to the Kernel image to run')
    args_parser.add_argument('-a',
                             '--kernel-arch',
                             help='Kernel Architecture',
                             default=SYSRV_DEFAULT_ARCH)

    return args_parser


def launch_qemu(kernel_path: str, kernel_arch: str):
    qemu_system_prog = f'qemu-system-{kernel_arch}'

    qemu_args = QEMU_FLAGS
    qemu_args.extend(['-kernel', kernel_path])

    _run_shell_cmd(qemu_system_prog, qemu_args)


def qemu_run(run_type: str, kernel_path: str, kernel_arch: str) -> None:
    # Set the build root
    env['SYSV_ROOT'] = cwd()

    # Build the System
    if run_type == 'launch':
        launch_qemu(kernel_path, kernel_arch)


if __name__ == "__main__":
    args = parse_arguments()

    qemu_run(**vars(args.parse_args()))
