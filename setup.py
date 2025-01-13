from __future__ import annotations

import os
from pathlib import Path

from setuptools import setup, Extension

file_dir = Path(__file__).parent.absolute().relative_to(Path().absolute())

piolib_root = Path("../utils/piolib")
piolib_headers = piolib_root.joinpath("include")

ext_modules = [
    Extension(
        name ="pio_counter._pio_counter",
        sources = [
            os.fspath(file_dir.joinpath("wrap_src", "pio_counter.c")),
            os.fspath(file_dir.joinpath("wrap_src", "counter_lib.c")),
            os.fspath(piolib_root.joinpath("piolib.c")),
            os.fspath(piolib_root.joinpath("pio_rp1.c")),
            
        ],
        language="c",
        extra_compile_args=[f"-I{piolib_headers}" ],
        extra_objects=[],
        include_dirs=[os.fspath(piolib_headers)],
    )
]


setup(
    ext_modules=ext_modules,
)
