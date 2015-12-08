#! /usr/bin/env python
import os
import shutil
import argparse
import tempfile


def main():

    parser = argparse.ArgumentParser(description='JetAnalysis config creation and run script.', add_help=True)
    parser.add_argument('pattern',help='Pattern.')
    parser.add_argument('replacement', help='Replacement.')
    parser.add_argument('files', type=str, nargs='+', help='Files.')

    args = vars(parser.parse_args())

    replace_dict = {}
    replace_dict[args['pattern']] = args['replacement']

    print replace_dict

    for filename in args['files']:
        replace(filename, replace_dict)

def replace(source_file_path, replace_dict):
    """ Replace all occurences of keys in replace_dict with the related
        value in the file source_file_path.
    """
    fh, target_file_path = tempfile.mkstemp()
    with open(target_file_path, 'w') as target_file:
        with open(source_file_path, 'r') as source_file:
            for line in source_file:
                for pattern in replace_dict:
                    line = line.replace('{0}'.format(pattern), replace_dict[pattern])
                target_file.write(line)
    os.remove(source_file_path)
    shutil.move(target_file_path, source_file_path)


if __name__ == '__main__':
    main()
