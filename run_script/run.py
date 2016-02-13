#!/usr/bin/env python
import subprocess
import sys
import time
import os
from gpiolib import *


def log(args, output, then, delta):
    timestamp = time.strftime("%c")
    if not os.path.exists("logs"):
        os.makedirs("logs")
    filename = "logs/" + timestamp + ".log"
    with open(filename, 'w') as f:
        f.write("==========================================================\n")
        f.write("Process: " + ' '.join(args) + '\n')
        f.write("Began: " + then + '\n')
        f.write("Took: " + str(delta) + " seconds to complete.\n")
        f.write("==========================================================\n")
        f.write("Process output:\n")
        f.write("==========================================================\n")
        f.write(output)
        f.write("==========================================================\n")


def main(pin):
    """Runs command given from commandline arguments once pin `pin` is
    turned on.

    Example call: sudo ./run.py ./exec arg1 arg2
    """
    try:
        sys_init()
    except:
        print("Could not initialize. Must be run as root.")
        sys.exit()

    pinMode(pin, INPUT)
    print "On pin", pin, "input, attempting to run: ", ' '.join(sys.argv[1:])
    while True:
        # if `pin` is ON, call an executable given by commandline arguments
        if digitalRead(pin):  # digitalRead(pin):
            # sys.argv is ["./run.py", "./test", "arg1", "arg2"]
            # [1:] strips off "./run.py"
            print "Running", ' '.join(sys.argv[1:])
            print "Changing script ownership."

            uid = int(os.environ.get('SUDO_UID'))
            gid = int(os.environ.get('SUDO_GID'))

            os.chown("./run.py", uid, gid)

            now = time.time()
            then = time.strftime("%c")
            # output = subprocess.check_output(sys.argv[1:])  # captures STDOUT
            proc = subprocess.Popen(sys.argv[1:],
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.STDOUT)

            if not os.path.exists("logs"):
                os.makedirs("logs")
            filename = "logs/" + str(datetime.datetime.now()).split('.')[0] + ".log"
            with open(filename, 'w') as f:
                f.write("==================================================\n")
                f.write(time.strftime("%c"))
                f.write("Process: " + ' '.join(args) + '\n')
                f.write("Began: " + then + '\n')
                f.write("Took: " + str(delta) + " seconds to complete.\n")
                f.write("==================================================\n")
                f.write("Process output:\n")
                f.write("==================================================\n")
                f.write(output)
                f.write("==================================================\n")
                for line in proc.stdout:
                    sys.stdout.write(line)
                    f.write(line)
                    proc.wait()
            # log(sys.argv[1:], output, then, time.time() - now)
            sys_clean()
            print "Exiting"
            sys.exit()
        else:
            pass
        time.sleep(0.1)  # 0.1 of a second

if __name__ == '__main__':
    main(27)
