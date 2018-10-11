### Create macros for the jobs

def CreateJobs(mac='WCSim'):
    script = '#!/bin/sh\n'
    curdir = '/users/zsoldos/hyperk/WCSim'
    script += 'cd {0}\n'.format(curdir)

    script += 'unset G4WORKDIR\n'
    script += 'export G4WORKDIR={0}/exe\n'.format(curdir)
    script += 'unset WCSIMDIR\n'
    script += 'export WCSIMDIR={0}\n'.format(curdir)

    datahome = '/data/zsoldos'
    script += 'source {0}/.cluster/root-v5-34-36/bin/thisroot.sh\n'.format(datahome)

    script += 'source {0}/.cluster/bin/setup_g410.sh\n'.format(datahome)

    script += 'export PYTHONPATH=$PYTHONPATH:/users/zsoldos/hyperk/WCSim/HKComputing/\n'
    script += 'WCSim {0}.mac\n'.format(mac)
    file = open('{0}.sh'.format(mac), 'w')
    file.write(script)
    file.close()

### Open SSH session and execute screen in a detach mode

import subprocess
import sys

from HKODWCSimMacro import HKODWCSimMacro

HOST="zsoldos@heppc400"
# Ports are handled in ~/.ssh/config since we use OpenSSH
COMMAND="uname -a"

ssh = subprocess.Popen(["ssh", "%s" % HOST, COMMAND],
                       shell=False,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)

result = ssh.stdout.readlines()
if result == []:
    error = ssh.stderr.readlines()
    print(sys.stderr, "ERROR: %s" % error)
else:
    print(result)


mac = HKODWCSimMacro()
mac.createHeader()
mac.createGeometry()
mac.createPMTsOptions()
mac.createDAQOptions()
mac.createODDAQOptions()
mac.createSandMuGenerator()
mac.createFooter()