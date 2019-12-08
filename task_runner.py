import docker
import subprocess
from time import sleep

client = docker.from_env()
container = client.containers.run("pack", command="webpack", detach=True, remove=True, cap_add=[
                                  "SYS_PTRACE"], security_opt=["seccomp=unconfined", "apparmor=unconfined"])
while True:
    pid = container.attrs['State']['Pid']
    print(container.attrs['State'])
    if pid == 0:
        sleep(0.1)
        container.reload()
        continue
    break
strace = subprocess.Popen(["strace", "-f", "-p", str(pid),  "-o", "webpack"])

container.exec_run(['touch', 'unlock'])
strace.wait()
