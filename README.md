RISC-V compiler toolchain code size benchmarks
==============================================

This repository accompanies the blog post [RISC-V Compiler Performance Part 1:
Code Size Comparisons](http://embecosm.com/blog/). All code and scripts used to
generate the data featured in the blog post is contained within this repository,
along with some extra configurations for architectures not discussed in the blog
post.

The scripts have been run on a Fedora 23 machine with Python 3.4. It is expected
that they will run unmodified on any Linux or OS X machine, but execution on
Windows will require some work (due to hard-coding folder separators as `/` and
path separators as `:`).

Re-running the benchmarks in your own setup will probably require some edits to
the `build_all.py` script, to suit your setup - in particular, there are the
following four variables to edit:

- `configurations`: This list names all the configurations that the benchmarks
  will be built for. If you wish to skip any of the configurations, simply
  delete them from this list.
- `toolchain_paths`: If any folders need prepending to the path for a given
  configuration, then edit the appropriate value in here. If no additional paths
  need prepending for a given configuration, use the empty string `''`.
- `toolchain_args`: The compiler flags for each configuration are given here.
  Some compilers require more flags than others in order to successfully
  compile for their target, but all use `-Os` in order to generate code
  optimised for size.
- `toolchain_cc`: This specifies the name of the compiler for each
  configuration.

Once you have made appropriate edits, you can build all outputs by running:

```
./build_all.py
```

`process.py` determines the code size of all objects and outputs a summary
table for all objects and configurations. If you have removed any of the
configurations from `build_all.py`, you will need to remove them from
`process.py` as well. If you only want to see a summary for a subset of
configurations, you can remove them from `process.py` as well.

To generate the summary, run:

```
./process.py
```


Feedback / Questions
--------------------

Please feel free to comment on the blog post, or email any questions or feedback
to [graham.markall@embecosm.com](mailto:graham.markall@embecosm.com).

If you have problems running the scripts or other difficulties, please open an
issue in this repository describing the problem and any relevant output.

Pull requests with any bug fixes or improvements (e.g. that add examples of
additional configurations) are welcomed.
