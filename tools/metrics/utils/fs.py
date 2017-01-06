from contextlib import contextmanager
import os


@contextmanager
def make_temp_dir(parent_dir):
    # create temp directory
    import tempfile
    temp_dir = tempfile.mkdtemp(dir=parent_dir)

    # return temporary directory
    yield temp_dir

    # cleanup parent directory
    import shutil
    os.chdir(parent_dir)
    shutil.rmtree(temp_dir)


def make_abspath_from_cwd(filename):
    return os.path.abspath(os.path.join(os.getcwd(), filename))


def print_to_file(filename, content):
    with open(filename, "w") as f:
        f.write(content)
