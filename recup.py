#!venv/bin/python3

"""Get the backup back."""

import sys
import shutil
from pathlib import Path

from src.exceptions import RecupDirError
from src.utilities import ciao



class RecupOptions:

    def __init__(self, bak_dir:Path, home_dir:Path):
        self.bak_dir = bak_dir
        self.home_dir = home_dir
        self.dir_jobs:list[DirJob] = []
        self.file_jobs:list[FileJob] = []


def bak_to_home(bak_path:Path, options:RecupOptions):
    """Return the home path corresponding to the given backup path."""
    rel_path = bak_path.relative_to(options.bak_dir)
    return options.home_dir / rel_path


def is_venv_link(filepath:Path)->bool:
    """Say if the given path is from a venv."""
    str_filepath = str(filepath)
    if 'venv' not in str_filepath:
        return False
    possibilities = [
        "venv/bin/python3.10",
        "venv/bin/python3",
        "venv/bin/python",
        "venv/lib64"
    ]
    for name in possibilities:
        if str(filepath).endswith(name):
            return True
    return False


class DirJob:

    def __init__(self, bak_dir:Path, options:'RecupOptions'):
        self.options = options
        self.bak_dir = bak_dir
        self.home_dir = bak_to_home(self.bak_dir, self.options)
        self.check()

    def check(self):
        """Check that the job is well formed."""
        if not self.bak_dir.is_dir():
            raise RecupDirError()

    def has_to_do(self)->bool:
        """Say if the directory has to be created."""
        if self.home_dir.is_dir():
            return False
        return True

    def run(self):
        self.home_dir.mkdir(parents=True, exist_ok=True)


class FileJob:

    def __init__(self, bak_file:Path, options:'RecupOptions'):
        self.options = options
        self.bak_file = bak_file
        self.home_file = bak_to_home(self.bak_file, self.options)
        self.check()

    def check(self):
        """Check that the job is well formed."""
        if not self.bak_file.is_file():
            raise RecupDirError()

    def has_to_do(self)->bool:
        """Say if the file has to be created."""
        if self.home_file.is_file():
            return False
        return True

    def run(self):
        print(f"Copy {self.bak_file} -> {self.home_file}")
        print("commenté pour éviter de faire des bêtises")
        # shutil.copy2(self.bak_file, self.home_file)
        _ = shutil


def fill_from_dir(bak_dir:Path, options:'RecupOptions'):
    """Fill the file and directory jobs from the given directory"""
    print(f"fichiers : {len(options.file_jobs)} -- dirs : {len(options.dir_jobs)}        ", end='\r')
    dir_job = DirJob(bak_dir, options)
    if dir_job.has_to_do():
        options.dir_jobs.append(dir_job)

    for elem in bak_dir.iterdir():
        if elem.is_symlink():
            if is_venv_link(elem):
                continue
            print("Il ne devrait pas y avoir de liens dans le backup.")
            print(f"Pourtant ceci est un lien : {elem}")
            raise RecupDirError()
        if elem.is_dir():
            fill_from_dir(elem, options)

        if elem.is_file():
            file_job = FileJob(elem, options)
            if file_job.has_to_do():
                options.file_jobs.append(file_job)


def run_jobs(options:'RecupOptions'):
    """Create the directorie and copy the files."""
    print("Create directories")
    total = len(options.dir_jobs)
    for num, dir_job in enumerate(options.dir_jobs):
        print(f"directory {num}/{total}", end="\r")
        dir_job.run()

    print("Create files")
    total = len(options.file_jobs)
    for num, file_job in enumerate(options.file_jobs):
        print(f"file {num}/{total}")
        file_job.run()


bak_dir = Path(sys.argv[1])
home_dir = Path(sys.argv[2])
ciao("attention aux noms des répertoires")
options = RecupOptions(bak_dir, home_dir)
fill_from_dir(bak_dir, options)

run_jobs(options)
