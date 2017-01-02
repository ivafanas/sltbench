from contextlib import contextmanager


@contextmanager
def make_temp_dir(parent_dir):
    # create temp directory
    import tempfile
    temp_dir = tempfile.mkdtemp(dir=parent_dir)

    # return temporary directory
    yield temp_dir

    # cleanup parent directory
    import os
    import shutil
    os.chdir(parent_dir)
    shutil.rmtree(temp_dir)


def print_to_file(filename, content):
    with open(filename, "w") as f:
        f.write(content)
