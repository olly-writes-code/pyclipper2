build:
  meson setup builddir --wipe

compile:
  meson compile -C builddir

install:
  uv pip install -e .

test:
  uv run pytest

refresh-and-test:
  just compile
  just install
  just test
