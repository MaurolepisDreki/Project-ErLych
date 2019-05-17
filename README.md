## Notes on our setup ##
1. The `ready` script is provided to simplify build system (re)configuration.  Yes, it's a stupid program, but it's smart enough to bring the project up to a more familiar state and make retorical comments when things go wrong.
2. `./configure` only has to be ran the first time to attain the Makefile.  The GNU Build System is smart enough that make will reexecute `./configure` when it is newer then the Makefile (eg after rerunning `./ready`).
3. `./ready` must be executed from within the source tree, but `./configure` should be ran from inside a subdirectory such as `build/` that the [human] compiler must first create, &amp;c.

