#! /usr/bin/env python3
#
# break the biclique file into single bicliques.

import sys
import os
SUFFIX = 'biclique'


if __name__ == '__main__':
    if len(sys.argv) > 4 or len(sys.argv) < 2:
        print('Break the biclique file into single bicliques.')
        sys.exit('Usage: ' + os.path.basename(sys.argv[0]) + ' [biclique file] [m-cutoff] [n-cutoff]')

    m, n = 0, 0
    if len(sys.argv) == 3:
        n = 0
    elif len(sys.argv) == 2:
        m, n = 0, 0
    else:
        m, n = int(sys.argv[2]), int(sys.argv[3])

    print('Cut-off values: %d, %d' % (m, n))
    print('Reading: %s.' % sys.argv[1])
    src = open(sys.argv[1], 'r').readlines()

    for i in range(0, len(src), 3):
        # print '%d' % (i/3)
        cliq_name = '%d.%s' % (i / 3, SUFFIX)
        # applying the filter now.
        if len(src[i].split()) >= m and len(src[i + 1].split()) >= n:

            print('Writing: %s.' % cliq_name)
            f = open(cliq_name, 'w')
            f.write(src[i])
            f.write(src[i + 1])
            f.close()
