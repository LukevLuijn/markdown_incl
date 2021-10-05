# Markdown include

See test docs for examples.

## Installation

```bash
git clone https://github.com/LukevLuijn/markdown_incl.git
cd markdown_incl/markdown_incl
mkdir build
cd build
cmake ..
make
```

## Alias

It is recommended to make an alias for quick use.

```bash

echo "$HOME/path/to/project/markdown_incl/markdown_incl/build/markdown_incl" >> ~/.bashrc
source ~/.bashrc
```

## Usage

Without alias

```bash
cd path/to/build/folder
./markdown_incl -s=path/to/source/document/source.md -o=path/to/output/document/output.md 
```

With alias

```bash
cd path/to/source/document
markdown_incl -s=source.md -o=output.md
```

## More options

```bash
./markdown_incl -h
```





