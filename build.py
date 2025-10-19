#
# Build invocation script for sysrv
#
# Copyright (C) 2025 Jesse Buhagiar
#

from argparse import ArgumentParser
from os import environ as env
from os import getcwd as cwd
from os import makedirs as mkdir
from sys import stdout
import subprocess

# The default architecture to build
SYSRV_DEFAULT_ARCH = 'rv64'
BUILD_TARGETS = ['build', 'clean']
BUILD_DIRECTORY = "build"


def _run_shell_cmd(command: str,
                   command_args: list[str]) -> bool:
    proc_args = [command]
    proc_args.extend(command_args)
    with subprocess.Popen(proc_args,
                          stdout=stdout,
                          cwd=env['SYSV_ROOT'],
                          text=True) as proc:
        try:
            output, errs = proc.communicate(timeout=30)

            if errs is not None:
                print(errs, flush=True)
        except subprocess.TimeoutExpired:
            proc.kill()


def parse_arguments() -> ArgumentParser:
    args_parser = ArgumentParser('System-RV Build Script')

    args_parser.add_argument('target',
                             choices=BUILD_TARGETS,
                             help='Possible build targets')
    args_parser.add_argument('-a',
                             '--kernel-arch',
                             help='Kernel Architecture',
                             default=SYSRV_DEFAULT_ARCH)

    return args_parser


def build_system() -> None:
    mkdir(BUILD_DIRECTORY, exist_ok=True)
    _run_shell_cmd('cmake', [f'-DCMAKE_TOOLCHAIN_FILE=meta/cmake/{env['SYSRV_KERNEL_ARCH']}.cmake',
                             '-B',
                             BUILD_DIRECTORY])
    _run_shell_cmd('ninja', ['-C', BUILD_DIRECTORY])


def clean_build() -> None:
    _run_shell_cmd('ninja', ['-C', BUILD_DIRECTORY, 'clean'])
    _run_shell_cmd('rm', ['-rf', './build'])


def do_build(target: str,
             kernel_arch: str) -> None:

    # Set the Kernel Architecture
    env['SYSRV_KERNEL_ARCH'] = kernel_arch

    # Set the build root
    env['SYSV_ROOT'] = cwd()

    # Build the System
    if target == 'build':
        build_system()
    # Run a clean command
    elif target == 'clean':
        clean_build()


if __name__ == "__main__":
    args = parse_arguments()

    do_build(**vars(args.parse_args()))
