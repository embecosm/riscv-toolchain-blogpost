#!/usr/bin/env python3

from glob import glob
from subprocess import check_output

# The list of configurations that will be summarised. Delete any from this list
# that you have not build with build_all.py, and and that you don't want to see
# the summary output for.
configurations = [ 'x86_64gcc', 'x86_64clang',
                   'riscv32gcc', 'riscv32clang',
                   'riscv64gcc', 'riscv64clang',
                   'armv7mgcc', 'armv8mgcc', 'armv8agcc' ]

objs = set()
results = {}

# Read all objects and find their code sizes

for config in configurations:
    sizes = {}
    for f in glob('out/*.%s' % config):
        # Only look at object files, not whole executables
        if '.o.' not in f:
            continue

        name = f.split('/')[1].split('.')[0]
        objs.add(name)

        # Get section headers
        output = check_output(['readelf', '-S', '-W', f])

        # Extract only the size of text sections, and not relocations (e.g.
        # .text but not .rela.text)
        for line in output.split(b'\n'):
            if b'.text' in line and b'rela' not in line:
                details = line.split(b']')[1]
                this_size = int(details.split()[4].strip(), 16)
                # Combine this size with the size of any previously-seen size
                size = sizes.get(name, 0)
                size += this_size
                sizes[name] = size

    results[config] = sizes


# Output summary table

print('Object\t%s' % ('\t'.join(configurations)))

for obj in sorted(objs):
    sizes = '\t'.join([ str(results[config][obj]) for config in configurations ])
    print('%s\t%s' % (obj, sizes))
