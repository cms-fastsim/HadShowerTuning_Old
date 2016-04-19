echo "mypwd=\$PWD" > setenv.sh
echo "cd $CMSSW_BASE/src" >> setenv.sh
echo "cmsenv" >> setenv.sh
echo "cd \$mypwd" >> setenv.sh
echo "source $ROOTSYS/bin/thisroot.sh" >> setenv.sh
if [ -n "${geant4_envbindir+1}" ]; then
    echo "source $geant4_envbindir/geant4.sh" >> setenv.sh
    mkdir DataGeneration-build
    cd DataGeneration-build
    echo "cmake -DGeant4_DIR=${geant4_envbindir}../ -DCMAKE_MODULE_PATH=$ROOTSYS/etc/cmake/ ../DataGeneration; make" > compile.sh
    cd $mypwd
    cd DataGeneration
    python createGeometry.py
    cd $mypwd
else
    echo "WARNING: geant4 environment is not set: we assume you won't need geant4"
fi
