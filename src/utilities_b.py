"""Some project-related utilities."""

import os
from pathlib import Path
from functools import cache

from src.getters import get_options
from src.options import Options
from src.stubs import LoraJob


def is_excluded_dir(my_path: Path) -> bool:
    """Say if a directory is excluded from the backup."""
    options = get_options()
    paths = options.paths
    _ = paths, my_path

    if my_path.name == "venv":
        exp_python = my_path / "bin" / "python"
        if exp_python.is_file():
            return True
    if my_path in paths.excluded_dirs:
        return True
    return False


def manage_crashs(fun):
    """Manage the case in which a job crashes."""

    def new_fun(self: LoraJob):
        try:
            fun(self)
        except OSError as error:
            print("Une erreur est survenue")
            print(error)
            self.options.jobs_manager.emergency_stop()
    return new_fun


class CloseThreads:
    """Colored output"""

    def __init__(self, options: Options):
        """Initialize."""
        self.options = options

    def __exit__(self, *args):
        """Reset all the colors"""
        _ = args
        self.options.jobs_manager.close_threads()

    def __enter__(self):
        """Enter the context manager."""
        self.options.jobs_manager.start()


def all_files(directory: Path):
    """Yiels all the files to be checked."""
    options = get_options()
    jobs_manager = options.jobs_manager
    for elem in directory.iterdir():
        if elem.is_symlink():
            continue
        if not jobs_manager.open:
            print("le manager est fermé")
            print("j'arrête le all_files")
            return
        if elem.is_file():
            yield elem
        if elem.is_dir():
            if is_excluded_dir(elem):
                continue
            yield from all_files(elem)


@cache
def is_empty_dir(directory:Path)->bool:
    """
    Say if a directory is empty.

    A directory is non empty if it contains a file in one of its subdirectory.
    """
    if not directory.is_dir():
        # There are sockets and so on.
        return False
    for elem in directory.iterdir():
        if elem.is_symlink():
            return False
        if elem.is_file():
            return False
        if elem.is_dir():
            if not is_empty_dir(elem):
                return False
    return True


def empty_subdirs(directory: Path):
    """Yield the emty subdirectories of the given one."""
    if os.access(directory, os.R_OK):
        return
    found_empties: list[Path] = []
    for elem in directory.iterdir():
        if elem.is_symlink():
            continue
        if elem.is_file():
            continue
        if elem.is_dir():
            yield from empty_subdirs(elem)
        if is_empty_dir(elem):
            found_empties.append(elem)
    if is_empty_dir(directory):
        yield directory
        return
    for subdir in found_empties:
        yield subdir
