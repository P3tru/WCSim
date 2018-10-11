### Create macros for the jobs

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