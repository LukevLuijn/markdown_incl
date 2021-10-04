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

## Script

The included [script](./script/markdown_incl.sh) helps with file names because the program takes absolute paths, you'll
need to change the path on line 7 to the actual path. It is recommended to make an alias for this script.

```bash
alias markdown_incl="bash ~/path/to/script/markdown_incl.sh $1 $2"
```

## Usage

```bash
./markdown_incl -h 
```


