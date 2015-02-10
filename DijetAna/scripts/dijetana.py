#!/usr/bin/env python

import os
import sys
import argparse
import copy
import importlib
import fileinput
from datetime import datetime
import shutil
import tempfile

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
                        help="Name of output file."
    parser.add_argument("-P", "--print-config", default=False, action="store_true",
                        help="Print out the JSON config before running Artus.")
    parser.add_argument('-f', '--fast', type=int, default=None,
                        help="limit number of input files or grid-control jobs. 3=files[0:3].")
    parser.add_argument("--no-run", default=False, action="store_true",
                        help="Exit before running Artus to only check the configs.")
    args = vars(parser.parse_args())

    # Prepare input files
    args['input files'] = expand_glob(args['input files'])
    if args['fast'] is not None:
        args['input files'] = args['input files'][:args['fast']
    print args['input files']

    nicknames = get_nicknames(args['input_files'])

    # If batch mode then no need to create configs no
    if args['batch']:
        wrapper.prepare_gc_input()

    runwrapper = RunWrapper()

    if args['config']
    if len(nicknames > 1):
    configs = {}

    for nickname in nicknames:
        config = get_config(args['inputconfig'], nick=nickname)
        config['InputFiles'] = get_filelist(args
        configs[nickname] = config

    if args['batch']:
        pass
        wrapper.prepare_gc_input()
    else:
        for nickname in wrapper.nicknames:
            config = dict(get_config(args['inputconfig'], nick=nickname).items() + config.items())
            wrapper.setConfig(config)
            wrapper.run()


def get_config(config_name, *args, **kwargs):
    """ Get config from templates by name."""
    if os.path.isfile(config_name): 
        config = ArtusConfig(
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
    """Returns nickname of syntac PREFIX_NICKNAME_SUFFIX."""
    return '_'.join(s.split('_')[1:-1])

def expand_glob(l):
    
    if (isinstance(l, basestring)):
        l = [l]
    expanded = []
    for item in l:
        if os.path.isdir(item):
            expanded.append(glob.glob(os.path.expandvars(item + '/*.root'))
        else:
            expanded.append(glob.glob(os.path.expandvars(item)))
    flattened = [val for sublist in expanded for val in sublist]

    return flattened





class RunWrapper(object):

    def __init__(self):

        self._config = 
        self.executable = 'JetAna'
        self.work_directory = '/nfs/dust/cms/user/gsieber/ARTUS'
        self.cmssw_directory = os.getenv('CMSSW_BASE')
        self.jetana_directory = os.path.join(self.cmssw_directory, 'src/JetAnalysis/DijetAna')

        self.nicknames = self.get_nicknames()
        self.date_now = datetime.now().strftime("%Y-%m-%d_%H-%M")
 


    def write_dbsfile(self, filename=None):
        """Write filenames ordered by nicknames to dbs file."""
        if filename is None:
            filename = "datasets_{0}.dbs".format(hashlib.md5(str(self._config)).hexdigest())
            filename = os.path.join(self.projectPath, dbsFileBasename)

        with open(filename, "w") as f:
            for nickname in self.nicknames:
                f.write("[{0}]\n".format(nickname))
                f.write("nickname = {0}\n".format(nickname))
                for filename in self._config["InputFiles"]:
                    if JetAnaWrapper.extract_nickname(filename) == nickname:
                        f.write("{0}\n".format(filename))


    def prepare_gc_input(self):
        """Prepare gridcontrol configs and work directory."""

        project_directory = os.path.join(self.work_directory, 'artus_{0}'.format(self.date_now))
        if not os.path.exists(project_directory):
            os.makedirs(project_directory)

        # Copy gc config template to project directory
        shutil.copy(os.path.join(self.jetana_directory, 'data/gc_template.conf'),
                    os.path.join(project_directory, 'jetana.conf'))

        # Copy run script to project directory
        shutil.copy(os.path.join(self.jetana_directory, 'data/run_jetana.sh'),
                    os.path.join(project_directory, 'run_jetana.conf'))


        # Write dbs file to project directory.
        dbs_filepath = os.path.join(project_directory, 'datasets.dbs')
        self.write_dbsfile(filename=dbs_filepath)

    def run_gc(self):
        pass

    def run_local(self):
        pass

    @staticmethod
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
