=== Building the project
To build the project, simply run `make doit`.

=== Examples
```
==> ls -l
total 129
-rwxrwx--x 1 bwhetherington bwhetherington  6866 Aug 30 16:25 a.out
drwxrws--x 2 bwhetherington bwhetherington    24 Aug 31 10:28 bin
-rwxrwx--x 1 bwhetherington bwhetherington 12407 Aug 31 10:28 doit
-rwxrwx--x 1 bwhetherington bwhetherington   179 Aug 30 16:24 foo.c
-rwxrwx--x 1 bwhetherington bwhetherington   663 Aug 31 10:24 Makefile
drwxrws--x 2 bwhetherington bwhetherington    48 Aug 30 16:21 src
------------------------------------------------------------
User CPU time:                  0 ms
System CPU time:                0 ms
Wallclock time:                 2 ms
Times involuntarily preempted:  7
Times voluntarily preempted:    8
Major page faults:              0
Minor page faults:              313
Maximum resident set size used: 1012
------------------------------------------------------------
Finished executing child process ls (13519) with status 0.
```

```
==> sleep 5
------------------------------------------------------------
User CPU time:                  0 ms
System CPU time:                1 ms
Wallclock time:                 5002 ms
Times involuntarily preempted:  8
Times voluntarily preempted:    11
Major page faults:              0
Minor page faults:              489
Maximum resident set size used: 1012
------------------------------------------------------------
Finished executing child process sleep (29621) with status 0.
```

```
==> set prompt = > 
> echo Hello world
Hello world
------------------------------------------------------------
User CPU time:                  0 ms
System CPU time:                1 ms
Wallclock time:                 0 ms
Times involuntarily preempted:  9
Times voluntarily preempted:    12
Major page faults:              0
Minor page faults:              655
Maximum resident set size used: 1012
------------------------------------------------------------
Finished executing child process echo (11019) with status 0.
```

=== Notes
Unfortunately, I did not have time to implement the 3rd part of the project,
background tasks. Unlike a typical Unix shell, mine does not implement many
features such as real environment variables, 