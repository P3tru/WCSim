from fabric import Connection, SerialGroup

### Need a config file with the local available computer
def openClusterCfg(file):
    return open(file,'r').read().split('\n')

###
def proc_free(c):
    ### First, compute nproc and put it on a variable
    ### Creer la commande
    nproc = c.run('nproc', hide=True)
    return nproc.stdout


def getServerGroup(cfgFile='QMULCluster.cfg'):
    serverList = openClusterCfg(cfgFile)
    serverGroup = SerialGroup()
    for server in serverList:
        serverGroup.append(Connection(server))
    return serverGroup


for cxn in getServerGroup():
    print("{}: {}".format(cxn, proc_free(cxn)))

def exec_script(c):
    uname = c.run('uname -s', hide=True)
    if 'Linux' in uname.stdout:
        command = "df -h / | tail -n1 | awk '{print $5}'"
        return c.run(command, hide=True).stdout.strip()


