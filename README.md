# IEEE-2016
Team Repository for the IEEE region 5 robotics competition.

Currently, this is where the framework for the autonomous locomotion, localization, and navigation code goes.

We have a [wiki](https://github.com/sdsmt-robotics/IEEE-2016/wiki) for this repo. It can be edited online and by cloning the *wiki* (not the IEEE-2016 repo, there's a separate one for the wiki) repository.

Our Makefile will attempt to compile any `.c` files in `src/` to object files in the same directory, which `g++` will then link together into an executable located in the same directory as the Makefile.

## Locomotion Status:

`driveWheelSteps( LEFT/RIGHT/BOTH, steps, time, serial_port );` works. Sort of. The left wheel goes backwards, and giving negative steps does nothing. If you put `driveWheelSteps` in an infinite loop, the first few calls do nothing, but subsequent calls work as expected given the above caviots. There was a bug in the Arduino side that caused some reliability issues, but it should be fixed now. Hopefully once the Arduino side can work with being given negative steps everything should iron itself out.
