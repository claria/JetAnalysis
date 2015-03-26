#!/usr/bin/env python

import argparse
import copy
from datetime import datetime
import fileinput
import glob
import importlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
import hashlib

from ConfigParser import RawConfigParser

from Artus.Configuration.artusWrapper import ArtusWrapper
from Artus.Utility import jsonTools

import logging
log = logging.getLogger(__name__)


def main():
    parser = argparse.ArgumentParser(description='JetAnalysis config creation and run script.', add_help=True)
    parser.add_argument('-c', '--config', type=str, default='RunConfig',
                        help='Config file name or template name to load config from.')
    parser.add_argument("-i", "--input-files", nargs="+", required=False,
                        help="Input root files.")
    parser.add_argument("-o", "--output-file", default=None,
                        help="Name of output file.")
    parser.add_argument("-p", "--print-config", default=False, action="store_true",
                        help="Print out the JSON config before running Artus.")
    parser.add_argument('-f', '--fast', type=int, default=None,
                        help="limit number of input files or grid-control jobs. 3=files[0:3].")
    parser.add_argument("--no-run", default=False, action="store_true",
                        help="Exit before running Artus to only check the configs.")
    parser.add_argument("-d", "--dry-run", default=False, action="store_true",
                        help="Exit before running Artus to only check the configs.")
    parser.add_argument("-s", "--save-config", default=None,
                        help="Write config to file. If not specified temp file is used.")
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

    # Empty filelist
    if not args['input_files']:
        raise ValueError("Input file list is empty.")

    # Unique list of nicknames of all input files
    nicknames = get_nicknames(args['input_files'])

    if args['batch']:
        work_directory = '/nfs/dust/cms/user/gsieber/ARTUS'
        project_directory = prepare_gc_input(args['input_files'], 
                                             config=args['config'], 
                                             work_directory=work_directory)

        gc_command = os.path.expandvars('$HOME/grid-control/go.py')
        arguments = '-Gc {0}'.format(os.path.join(project_directory, 'jetana.conf'))
        try:
            run(gc_command, arguments=arguments)
        except KeyboardInterrupt:
            pass
        print 'grid-control was invoked with cmd: \"{0} {1}\"'.format(gc_command, arguments)
        print 'Output was written to \"{0}\"'.format(project_directory)

    if args['batch'] is not True:
        # Prepare list of all configs
        # TODO Fix output filename
        configs = []
        for nickname in nicknames:
            config = get_config(args['config'], nick=nickname)
            config['InputFiles'] = [filename for filename in args['input_files'] if extract_nickname(filename) == nickname]
            if args['output_file'] is None: 
                config['OutputFile'] = "{0}_{1}".format('output', nickname)
            else:
                config['OutputFile'] = args['output_file']
            config['nickname'] = nickname
            configs.append(config)
        # Run over each config
        for config in configs:
            if args['print_config']:
                print json.dumps(config, sort_keys=True, indent=4)
                continue
            path = save_config(config, path=args['save_config'])
            rc = run("JetAna", arguments=path)
            if rc != 0:
                raise Exception("Error in called program")



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
        path = os.path.join(tempfile.gettempdir(), basename)
    with open(path, "w") as f:
        json.dump(config, f, indent=indent, sort_keys=True)

    log.debug("Config written to \"{0}\"".format(path))
    return path


def get_hash(s, truncate=12):
    """Return a (truncated) hash of the input string."""
    return hashlib.md5(s).hexdigest()[0:truncate]

def get_config(config_name, *args, **kwargs):
    """ Get config from templates by name."""
    from JetAnalysis.DijetAna import artusconfigs
    try:
        config_class = getattr(artusconfigs, config_name)
    except AttributeError:
        log.critical( "The config {0} does not exist or is not a valid config.".format(config_name))
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
        item = item.strip().strip(',')
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
                    f.write("{0} = 1\n".format(filename))
    log.debug('Wrote dbs file to work directory.')


def prepare_gc_input(filelist, config, work_directory):
    """Prepare gridcontrol configs and work directory."""

    date_now = datetime.now().strftime("%Y-%m-%d_%H-%M")
    cmssw_directory = os.getenv('CMSSW_BASE')
    jetana_directory = os.path.join(cmssw_directory, 'src/JetAnalysis/DijetAna')


    project_directory = os.path.join(work_directory, 'artus_{0}'.format(date_now))
    if not os.path.exists(project_directory):
        os.makedirs(project_directory)
    log.debug('Created work directory \"{0}\"'.format(project_directory))

    # Copy gc config template to project directory
    gc_config_path = os.path.join(project_directory, 'jetana.conf') 
    shutil.copy(os.path.join(jetana_directory, 'data/gc_template.conf'), gc_config_path)
    log.debug('Copied gc template file to work directory.')


    # Copy run script to project directory
    shutil.copy(os.path.join(jetana_directory, 'data/run_jetana.sh'),
                os.path.join(project_directory, 'run_jetana.sh'))
    log.debug('Copied run script to work directory.')


    # Write dbs file to project directory.
    dbs_filepath = os.path.join(project_directory, 'datasets.dbs')
    write_dbsfile(filelist, path=dbs_filepath)

    # Replace variables in gc template
    replace_dict = {}
    replace_dict['CMSSW_BASE'] = cmssw_directory
    replace_dict['DBS_PATH'] = dbs_filepath
    replace_dict['CONFIG'] = config
    replace_dict['PROJECT_DIR'] = project_directory
    replace(gc_config_path, replace_dict)

    return project_directory

def replace(source_file_path, replace_dict):
    fh, target_file_path = tempfile.mkstemp()
    with open(target_file_path, 'w') as target_file:
        with open(source_file_path, 'r') as source_file:
            for line in source_file:
                for pattern in replace_dict:
                    line = line.replace('@{0}@'.format(pattern), replace_dict[pattern])
                target_file.write(line)
    os.remove(source_file_path)
    shutil.move(target_file_path, source_file_path)



if __name__ == '__main__':
    main()
