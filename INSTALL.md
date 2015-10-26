# Installation

**note**: these instructions are under construction

## Get package

```
git clone git@github.com:cms-fastsim/HadShowerTuning.git
```

## CMSSW environment

We use a CMSSW environment to ensure a consistent environment with all required libraries and header files, ROOT installation, ...

```
cmsrel CMSSW_7_4_0
cd CMSSW_7_4_0/src
cmsenv
```

**Note**: things will also work with many other CMSSW versions.

## Geant4 installation (optional)

**Note**: 
   * make sure the CMSSW is setup as described above
   * only install Geant4 if you want to generate tuning data.
   * the Geant4 installation will require about 1 GB of storage
   * there might be a way to use the geant4 install provided by CMSSW. This option has not been fully explored yet.

Pick/create a directory in which you want to install Geant4 and move to this directory,

Download version 10.01.p02 of the geant4 source code [from here](http://geant4.web.cern.ch/geant4/support/download.shtml)
and store it in your current directory (filename geant4.10.01.p02.tar.gz).

Install

```
source /path/to/.../HadShower/installGeant4.sh
```

Now you should have geant4 installed under $PWD/geant4.10.01.p02-install

**Note**: things will also work with many other Geant4 versions,
but you'll have to edit the installGeant4.sh file to account for the different version number in paths.

## Setup the HadShowerTuning package

   * make sure the CMSSW environment is properly setup
```
cd /path/to/.../CMSSW_7_4_0/src
cmsenv
cd -
```

   * (optional:) if you have geant4 installed, setup the geant4 environment
```
source /path/to/.../geant4.10.01.p02-install/bin/geant4.sh
```

   * run the setup script in HadShowerTuning

```
cd /path/to/.../HadShowerTuning/
source setup.sh
```

Now you should have a file setenv.sh.
Whenever you run this script,
your environement will be ready to
compile and run the code in HadShowerTuning.
