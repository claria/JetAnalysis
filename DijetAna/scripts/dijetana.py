#!/usr/bin/env python

import os
import sys
import argparse
import copy
import importlib
from ConfigParser import RawConfigParser

from Artus.Configuration.artusWrapper import ArtusWrapper

import logging
log = logging.getLogger(__name__)

# Change envs for artus run
artus_dir = os.path.join(os.path.expandvars('$CMSSW_BASE'), 'src/Artus')
os.environ['PATH'] += ':{0}/grid-control'.format(os.path.expandvars('$HOME'))
os.environ['ARTUS_WORK_BASE'] = '/nfs/dust/cms/user/gsieber/ARTUS'
os.environ['ARTUSPATH'] = artus_dir


def main():

    parser = argparse.ArgumentParser(description='Config file to load.', add_help=False)
    parser.add_argument('--artusconfig', type=str, default='RunConfig',
                   help='Name of Config file to load.')

    wrapper = ArtusWrapper('JetAna', userArgParsers=[parser])

    # Identify input type based on nick
    nick = wrapper.determineNickname('auto')

    log.info("Preparing config for nickname {}.".format(nick))

    args = vars(wrapper._args)
    config = wrapper.getConfig()
    log.info("Print Config")
    log.info(config)
    config = dict(get_config(args['artusconfig'], nick=nick).items() + config.items())

    wrapper.setConfig(config)
    sys.exit(wrapper.run())


def get_config(config_name, *args, **kwargs):
    from JetAnalysis.DijetAna import artusconfigs
    try:
        config_class = getattr(artusconfigs, config_name)
    except AttributeError:
        print "The config {0} does not exist or is not a valid config.".format(config_name)
        sys.exit(1)

    return config_class(*args, **kwargs)


if __name__ == '__main__':
    main()
