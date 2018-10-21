class HKEnv(object):

    def __init__(self):
        self.env = '#!/bin/bash -x\n'

        self.env += 'unset WCSIMDIR\n'
        self.env += 'export WCSIMDIR=/data/zsoldos/ProdV2\n'

        self.env += 'unset G4WORKDIR\n'
        self.env += 'export G4WORKDIR=$WCSIMDIR/exe\n'

        self.env += 'source $DATAHOME/.cluster/root-v5-34-36/bin/thisroot.sh\n'
        self.env += 'source $DATAHOME/.cluster/bin/setup_g410.sh\n'
        self.env += 'export PYTHONPATH=$PYTHONPATH:$WCSIMDIR/HKComputing/\n'

        self.envFileName = 'HKEnv'


    def close_file(self):
        file = open('{0}.sh'.format(self.envFileName), 'w')
        file.write(self.envFileName)
        file.close()
