# HadShowerTuning : Data Analysis

## example python scripts

Remember to setup root before you try this.
If things don't work, it might be that you have ROOT installed without pyROOT.
See [https://root.cern.ch/pyroot](pyroot page)

```
# create longitudinal profiles and sub-profiles for individual showers
# store as histograms
python longprofiles.py  dataset.root output.root

# draw distributions of energy fractions,
# e.g. fraction of energy carried by early (late) neutral pions
python pi0fraction.py dataset.root output.root
```
