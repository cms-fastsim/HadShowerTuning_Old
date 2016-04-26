mypwd=$PWD
G4VERSION="10.01.p02"
wget http://geant4.cern.ch/support/source/geant4.$G4VERSION.tar.gz
tar -xzf geant4.$G4VERSION.tar.gz
mkdir geant4.$G4VERSION-build
cd geant4.$G4VERSION-build
cmake \
-DCMAKE_INSTALL_PREFIX=$mypwd/geant4.$G4VERSION-install \
-DGEANT4_USE_OPENGL_X11=ON \
-DGEANT4_INSTALL_DATA=ON \
-DGEANT4_USE_GDML=ON \
-DXERCESC_LIBRARY=$CMSSW_RELEASE_BASE/../../../external/xerces-c/2.8.0-cms/lib/libxerces-c.so \
-DXERCESC_INCLUDE_DIR=$CMSSW_RELEASE_BASE/../../../external/xerces-c/2.8.0-cms/include/ \
-DGEANT4_BUILD_CXXSTD=c++11 \
$mypwd/geant4.$G4VERSION
make -j 12
make install
