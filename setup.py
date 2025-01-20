from __future__ import annotations

import os
from pathlib import Path
from setuptools import setup, Extension, find_packages

file_dir = Path(__file__).parent.absolute().relative_to(Path().absolute())

piolib_headers = Path("./include")

ext_modules = [
    Extension(
        name ="pio_counter._pio_counter",
        sources = [
            os.fspath(file_dir.joinpath("wrap_src", "pio_counter.c")),
            os.fspath(file_dir.joinpath("wrap_src", "counter_lib.c")),
        ],
        language="c",
        extra_compile_args=[f"-I{piolib_headers}"],
        extra_link_args=[],
        extra_objects=[file_dir.joinpath("python_src", "pio_counter", "libpio_lib.so")],
        include_dirs=[os.fspath(piolib_headers)],
    )
]
setup(
    packages=find_packages(where="python_src"),
    package_dir={"": "python_src"},
    package_data={"pio_counter": ["*.so"]},
    include_package_data=True,
    ext_modules=ext_modules,
)
