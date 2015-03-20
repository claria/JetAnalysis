#!/usr/bin/env python

import os
import sys
import argparse
import copy
from datetime import datetime
import importlib
import json
import fileinput
import glob
import shutil
import subprocess
import tempfile
import hashlib

from ConfigParser import RawConfigParser

from Artus.Configuration.artusWrapper import ArtusWrapper
from Artus.Utility import jsonTools

import logging
log = logging.getLogger(__name__)

# Change envs for artus run
artus_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'), 'src/Artus')
os.environ['PATH'] += ':{0}/grid-control'.format(os.path.expandvars('$HOME'))
os.environ['ARTUS_WORK_BASE'] = '/nfs/dust/cms/user/gsieber/ARTUS'
os.environ['ARTUSPATH'] = artus_dir


def main():

    parser = argparse.ArgumentParser(description='JetAnalysis config creation and run script.', add_help=True)
    parser.add_argument('--config', type=str, default='RunConfig',
                        help='Config file name or template name to load config from.')
    parser.add_argument("-i", "--input-files", nargs="+", required=True,
                        help="Input root files.")
    parser.add_argument("-o", "--output-file", default="output.root",
                        help="Name of output file.")
    parser.add_argument("-p", "--print-config", default=False, action="store_true",
                        help="Print out the JSON config before running Artus.")
    parser.add_argument('-f', '--fast', type=int, default=None,
                        help="limit number of input files or grid-control jobs. 3=files[0:3].")
    parser.add_argument("--no-run", default=False, action="store_true",
                        help="Exit before running Artus to only check the configs.")
    parser.add_argument("-d", "--dry-run", default=False, action="store_true",
                        help="Exit before running Artus to only check the configs.")
    parser.add_argument("-b", "--batch", default=False, action="store_true",
                        help="Submit to batch system using gc.")
    parser.add_argument("--log-level", default="info",
                        help="Log level.")

    args = vars(parser.parse_args())

    # Set log level
    log_level = getattr(logging, args['log_level'].upper(), None)
    if not isinstance(log_level, int):
        raise ValueError('Invalid log level: %s' % loglevel)
    logging.basicConfig(format='%(message)s',
                        level=log_level)

    # Prepare input files
    args['input_files'] = expand_glob(args['input_files'])
    if args['fast'] is not None:
        args['input_files'] = args['input_files'][:args['fast']]

    # Unique list of nicknames of all input files
    nicknames = get_nicknames(args['input_files'])

    if args['batch']:
        work_directory = '/nfs/dust/cms/user/gsieber/ARTUS'
        prepare_gc_input(args['input_files'], work_directory)


    # Prepare list of all configs
    configs = []
    for nickname in nicknames:
        config = get_config(args['config'], nick=nickname)
        config['InputFiles'] = [filename for filename in args['input_files'] if extract_nickname(filename) == nickname]
        config['OutputFile'] = "{0}_{1}".format(args['output_file'], nickname)
        config['nickname'] = nickname
        configs.append(config)

    # Run over each config or send to batch system
    if not args['batch']:
        for config in configs:
            path = save_config(config)
            run("JetAna", arguments=path)



def run(executable, arguments=''):
    """Execute and wait for command to complete. Returns returncode attribute."""
    cmd = '{0} {1}'.format(executable, arguments)
    log.debug("Executing command: \"{0}\"".format(cmd))
    try:
        rc = subprocess.call(cmd.split())
    except KeyboardInterrupt:
        log.critical("Received Interrupt")
        return 1
    return rc

def save_config(config, path=None, indent=4):
    """Save json config to file."""
    if path is None:
        basename = "artus_{0}.json".format(get_hash(str(config)))
        filepath = os.path.join(tempfile.gettempdir(), basename)
    with open(filepath, "w") as f:
        json.dump(config, f, indent=indent, sort_keys=True)

    log.debug("Config written to \"{0}\"".format(filepath))
    return filepath


def get_hash(s, truncate=12):
    """Return a (truncated) hash of the input string."""
    return hashlib.md5(s).hexdigest()[0:truncate]

def get_config(config_name, *args, **kwargs):
    """ Get config from templates by name."""
    from JetAnalysis.DijetAna import artusconfigs
    try:
        config_class = getattr(artusconfigs, config_name)
    except AttributeError:
        print "The config {0} does not exist or is not a valid config.".format(config_name)
        sys.exit(1)

    return config_class(*args, **kwargs)


def get_nicknames(filelist):
    """Return unique list of nicknames."""
    nicknames = []
    for path in filelist:
        basename = os.path.basename(path)
        nickname = extract_nickname(basename)
        nicknames.append(nickname)
    return list(set(nicknames))


def extract_nickname(s):
    """Returns nickname of syntax PREFIX_NICKNAME_SUFFIX."""
    s = os.path.basename(s)
    return '_'.join(s.split('_')[1:-1])

def expand_glob(l):
    """Expand and glob input list and return flattened list."""
    if (isinstance(l, basestring)):
        l = [l]
    expanded = []
    for item in l:
        if os.path.isdir(item):
            expanded.append(glob.glob(os.path.expandvars(item + '/*.root')))
        else:
            expanded.append(glob.glob(os.path.expandvars(item)))
    # Flatten list of lists and return
    return [val for sublist in expanded for val in sublist]





def write_dbsfile(filelist, path=None, work_directory=None):
    """Write filenames ordered by nicknames to dbs file."""
    nicknames = get_nicknames(filelist)

    if path is None:
        path = "datasets_{0}.dbs".format(get_hash(str(config)))
        path = os.path.join(work_directory, path)

    with open(path, "w") as f:
        for nickname in nicknames:
            f.write("[{0}]\n".format(nickname))
            f.write("nickname = {0}\n".format(nickname))
            for filename in filelist:
                if extract_nickname(filename) == nickname:
                    f.write("{0}\n".format(filename))
    log.debug('Wrote dbs file to \"{0}\"'.format(path))


def prepare_gc_input(filelist, work_directory):
    """Prepare gridcontrol configs and work directory."""

    date_now = datetime.now().strftime("%Y-%m-%d_%H-%M")
    cmssw_directory = os.getenv('CMSSW_BASE')
    jetana_directory = os.path.join(cmssw_directory, 'src/JetAnalysis/DijetAna')


    project_directory = os.path.join(work_directory, 'artus_{0}'.format(date_now))
    if not os.path.exists(project_directory):
        os.makedirs(project_directory)
    log.debug('Created work directory \"{0}\"'.format(project_directory))

    # Copy gc config template to project directory
    shutil.copy(os.path.join(jetana_directory, 'data/gc_template.conf'),
                os.path.join(project_directory, 'jetana.conf'))
    log.debug('Copied gc template file to work directory.')

    # Copy run script to project directory
    shutil.copy(os.path.join(jetana_directory, 'data/run_jetana.sh'),
                os.path.join(project_directory, 'run_jetana.conf'))
    log.debug('Copied run script to work directory.')


    # Write dbs file to project directory.
    dbs_filepath = os.path.join(project_directory, 'datasets.dbs')
    write_dbsfile(filelist, path=dbs_filepath)

def replace(source_file_path, replace_dict):
    fh, target_file_path = tempfile.mkstemp()
    with open(target_file_path, 'w') as target_file:
        with open(source_file_path, 'r') as source_file:
            for line in source_file:
                for pattern in replace_dict:
                    target_file.write(line.replace(pattern, replace_dict[pattern]))
    os.remove(source_file_path)
    shutil.move(target_file_path, source_file_path)



if __name__ == '__main__':
    main()
