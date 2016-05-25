#!/usr/bin/env python3

from glob import glob
from subprocess import check_output

arches = [ 'x86_64gcc', 'x86_64clang',
           'riscv32gcc', 'riscv32clang',
           'riscv64gcc', 'riscv64clang',
           'armv7mgcc', 'armv8mgcc', 'armv8agcc' ]
objs = set()

results = {}

for arch in arches:
    sizes = {}
    for f in glob('out/*.%s' % arch):
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

    results[arch] = sizes

print('Object\t%s' % ('\t'.join(arches)))

for obj in sorted(objs):
    sizes = '\t'.join([ str(results[arch][obj]) for arch in arches ])
    print('%s\t%s' % (obj, sizes))
