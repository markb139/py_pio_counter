from __future__ import annotations

import os
from pathlib import Path

from setuptools import setup, Extension

file_dir = Path(__file__).parent.absolute().relative_to(Path().absolute())

piolib_root = Path("../utils/piolib")
piolib_headers = piolib_root.joinpath("include")
piolib_lib = Path("/usr/local/lib")

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
        extra_objects=[piolib_lib.joinpath("libpio_lib.so")],
        include_dirs=[os.fspath(piolib_headers)],
    )
]

setup(
    ext_modules=ext_modules,
)
