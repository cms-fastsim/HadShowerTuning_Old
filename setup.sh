echo mypwd=$PWD > setenv.sh
echo "cd $CMSSW_BASE/src" >> setenv.sh
echo "cmsenv" >> setenv.sh
echo "cd \$mypwd" >> setenv.sh
if [ -n "${geant4_envbindir+1}" ]; then
    echo "source $geant4_envbindir/geant4.sh" >> setenv.sh
else
    echo "WARNING: geant4 environment is not set: we assume you won't need geant4"
fi
