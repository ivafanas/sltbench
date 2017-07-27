# How to benchmark

Producing stable performance results is a quite tricky task.
First of all, make sure that (**all points are critical for stable performance results**):
* The computer you are using is as much clean as possible.
  No browser is running, no background updates, no deamons / services (like databases, antivirus,
  background email fetching and so on).
* There is no other users logged in to the computer.
* Intel SpeedStep and Intel Turbo technologies are turned off (check your BIOS settings).
* Disable CPU scaling ([instructions](https://askubuntu.com/questions/3924/disable-ondemand-cpu-scaling-daemon/3927))
* You are NOT running from Visual Studio or another IDE (open separate console).
* The benchmark process is pinned to a single CPU core and this is not #0 core.
  It is important to get rid of task switching between CPU cores, so pin your process.
  In some systems OS preferably occupies core #0, benchmark should avoid to struggle with OS for
  resources. For windows follow
  [this instructions](http://www.howtogeek.com/121775/how-to-force-windows-applications-to-use-a-specific-cpu/)
  to pin process to core. For linux use `taskset` and `KMP_AFFINITY`.
* It is preferable to use `runlevel 1` for linux systems to make sure that no services is running
  and no more users is connected.
