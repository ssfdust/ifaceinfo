from distutils.core import Extension, setup

iface = Extension(
    "ifaceinfo",
    sources=["src/ifaceinfo.c", "src/ifip.c", "src/list.c"],
    include_dirs=["src/"],
)

setup(
    name="ifaceinfo",
    author="ssfdust",
    author_email="ssfdust@gmail.com",
    url="https://github.com/ssfdust",
    version="0.0.1",
    description="interface c extension for python",
    ext_modules=[iface],
)
