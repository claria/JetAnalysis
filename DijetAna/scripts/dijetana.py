#!/usr/bin/env python
"""
Usage: dijetana.py -i kappa_skim.root -c RunConfig -o output.root

Prepares the json config using the RunConfig template, sets the
input file kappa_skim.root and output file output.root and runs
the artus binary directly.
"""
import argparse
import copy
from datetime import datetime
import fileinput
import glob
import importlib
import json
import os
import signal
import shutil
import subprocess
import sys
import tempfile
import hashlib
from sys import stdin

from ConfigParser import RawConfigParser

from Artus.Configuration.artusWrapper import ArtusWrapper
from Artus.Utility import jsonTools

import logging
log = logging.getLogger(__name__)


def main():
    """Wrapper for the Artus binary call.

       Prepares the json config from config files or config classes. Then
       either calls directly the artus binary or prepares a grid-control
       config for a batch run.

    """

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
        if args['dry_run']:
            log.debug('Exit since only dry run requested.')
            pass
        else:
            run(gc_command, arguments=arguments)
        # Creates a symlink 'latest' in work directory pointing to project directory
        dest = os.path.join(work_directory, 'latest')
        if os.path.lexists(dest):
            os.remove(dest)
        os.symlink(project_directory, dest )

        log.info('Updated symlink to latest Artus run.')
        log.info('grid-control was invoked with cmd: \"{0} {1}\"'.format(gc_command, arguments))
        log.info('Output was written to \"{0}\"'.format(project_directory))


    if args['batch'] is not True:
        # Prepare list of all configs
        # TODO Fix output filename
        configs = []
        for nickname in nicknames:
            config = get_config(args['config'], nick=nickname)
            config['LogLevel'] = args['log_level']
            config['InputFiles'] = [filename for filename in args['input_files'] if extract_nickname(filename) == nickname]
            if args['output_file'] is None:
                config['OutputPath'] = "{0}_{1}".format('output', nickname)
            else:
                config['OutputPath'] = args['output_file']
            config['nickname'] = nickname
            configs.append(config)
        # Run over each config
        for config in configs:
            if args['print_config']:
                print json.dumps(config, sort_keys=True, indent=4)
                continue
            path = save_config(config, path=args['save_config'])
            if args['dry_run']:
                log.info("Config written to \"{0}\"".format(path))
                log.debug('Exit since only dry run requested.')
                pass
            else:
                rc = run("JetAna", arguments=path)
                if rc != 0:
                    log.info("Config written to \"{0}\"".format(path))
                    raise Exception("Error in called program")




def get_tty_fg():
    # new process group
    os.setpgrp()
    # don't stop the process when we get SIGTTOU. since this is now in a
    # background process group, SIGTTOU will be sent to this process when
    # we call tcsetpgrp(), below. the default action when receiving that
    # signal is to stop (process mode T).
    hdlr = signal.signal(signal.SIGTTOU, signal.SIG_IGN)
    # open a file handle to the current tty
    if stdin.isatty():
        tty = os.open('/dev/tty', os.O_RDWR)
        os.tcsetpgrp(tty, os.getpgrp())
    # replace the old signal handler to minimize the chance of the child
    # getting confused by a non-standard starting signal table.
    signal.signal(signal.SIGTTOU, hdlr)

def run(executable, arguments='',ncurses=False):
    """Execute and wait for command to complete. Returns returncode attribute."""

    cmd = '{0} {1}'.format(executable, arguments)
    log.debug("Executing command: \"{0}\"".format(cmd))
    try:
        # TODO start process in new console so that output does not get spoiled
        # subprocess.popen([sys.executable, 'script.py'], creationflags = subprocess.CREATE_NEW_CONSOLE)
        rc = subprocess.call(cmd.split(), preexec_fn=get_tty_fg)
    except KeyboardInterrupt:
        log.critical("Received Interrupt")
        rc = 1
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
    """Get config from file or from config templates by name."""

    if os.path.exists(config_name):
        log.info('Try to read config from file {0}'.format(config_name))
        with open(config_name) as json_file:
            try:
                config = json.load(json_file)
            except ValueError:
                log.critical('Failed to parse json file {0}'.format(config_name))
    else:
        from JetAnalysis.DijetAna import artusconfigs
        try:
            config_class = getattr(artusconfigs, config_name)
            config = config_class(*args, **kwargs)
        except AttributeError:
            log.critical( "The config {0} does not exist or is not a valid config.".format(config_name))
            sys.exit(1)

    return config


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
    # Create work directory for gc
    os.mkdir(os.path.join(project_directory, 'work.{0}'.format('jetana')))
    shutil.copy(os.path.join(jetana_directory, 'data/gc_template_user.conf'), gc_config_path)
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
    """ Replace all occurences of keys in replace_dict with the related
        value in the file source_file_path.
    """

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
