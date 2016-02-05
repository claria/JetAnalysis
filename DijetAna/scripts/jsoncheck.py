#!/usr/bin/env python

import json
import sys

with open(sys.argv[1]) as f:
    try:
        json.load(f)
    except ValueError as e:
        print('invalid json: %s' % e)
        sys.exit(1)

    print 'Json syntax validated succesfully.'
