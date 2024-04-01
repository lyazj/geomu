Install GENIE
--------------------

(1) Install GSL and Pythia6 (https://github.com/alisw/pythia6/tree/alice/428, [**IMPORTANT**] branch: alice/428), then execute:

    $ sudo mv /usr/local/lib/libpythia6.so /usr/local/lib/libPythia6.so

(2) Build ROOT [**IMPORTANT**] with MathMore and Pythia6 from scratch like:

    $ cmake .. -Dmathmore=ON -Dpythia6=ON

(3) Build LHAPDF6.

(4) Install HepMC3 and build Pythia8 with HepMC3, LHAPDF6, and gzip supports like:

    $ ./configure --prefix=/usr/local --with-hepmc3=/usr --with-lhapdf6=/usr/local --with-gzip=/usr

(5) Install log4cpp, xml2, Geant4.

(6) Build GENIE with Pythia6, Pythia8, Geant4, ROOT, LHAPDF6, log4cpp, and xml2 supports like:

    $ export GENIE="${PWD}"
    $ export ROOTSYS=/usr/local
    $ ./configure --enable-pythia8 --enable-geant4 --disable-lhapdf5 --enable-lhapdf6 --with-pythia6-lib=/usr/local/lib

An example environment configuration for GENIE:

    export GENIE=/home/lyazj/software/genie_generator
    export ROOTSYS=/usr/local
    export LHAPATH=/usr/local/share/LHAPDF
    export PATH=${PATH}:${GENIE}/bin
    if [ -z "${LD_LIBRARY_PATH}" ]; then
        export LD_LIBRARY_PATH=${GENIE}/lib
    else
        export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${GENIE}/lib
    fi
    export LD_PRELOAD=/usr/local/lib/libG4geometry.so:/usr/local/lib/libG4processes.so

Reference versions:

    (on Linux 5.15.146.1-microsoft-standard-WSL2 x86_64 x86_64 x86_64 GNU/Linux)
    GSL              2.7.1
    Pythia6          6.428
    ROOT             6.30.04
    LHAPDF6          6.5.4
    libhepmc3-dev    3.1.2-2build1
    zlib1g-dev       1:1.2.11.dfsg-2ubuntu9.2
    Pythia8          8.311
    liblog4cpp5-dev  1.1.3-3
    libxml2-dev      2.9.13+dfsg-1ubuntu0.4
    Geant4           11.2.1
    GENIE            3.04.00
