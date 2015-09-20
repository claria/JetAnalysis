JetAnalysis
===========

[![Build Status](https://travis-ci.org/claria/JetAnalysis.svg?branch=master)](https://travis-ci.org/claria/JetAnalysis)

## Installation

### Software Requirements
- CMSSW (provides ROOT, Python etc.)
- RooUnfold
- Artus
- Kappa
- KappaTools


### Software Requirements
All software requirements as well as the JetAnalysis package can be installed
via the `checkout.sh` script in the base folder or directly with the following
commands.

```
wget https://github.com/claria/JetAnalysis/raw/master/checkout.sh
chmod +x checkout.sh && ./checkout.sh
```

## Usage

### Skimming

The skimming configs for Kappa are in a separate repository to be found here:

```
https://github.com/claria/JetSkims
```

### Analysis

The main analysis can be run with the command

```
dijetana.py -i JET_2012*.root -o output.root -c RunConfig
```

#### Int. Luminosity

    Use pixelLumiCalc.py tool to calculate the integrated luminosity for the 2012 run.

```pixelLumiCalc.py overview -i ${CMSSW_BASE}/src/JetAnalysis/DijetAna/data/json/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt```

### Plotting
