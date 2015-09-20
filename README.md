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


```
wget https://github.com/claria/JetAnalysis/raw/master/checkout.sh
chmod +x checkout.sh && ./checkout.sh
```
## Usage

### Skimming

### Analysis

#### Int. Luminosity

    Use pixelLumiCalc.py tool to calculate the integrated luminosity for the 2012 run.

```pixelLumiCalc.py overview -i ${CMSSW_BASE}/src/JetAnalysis/DijetAna/data/json/Cert_190456-208686_8TeV_22Jan2013ReReco_Collisions12_JSON.txt```

### Plotting
