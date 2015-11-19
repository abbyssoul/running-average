# running-average
Compute running average reading from standard input stream

## Description 
This is a simple command line utility to compute a [https://en.wikipedia.org/wiki/Moving_average running average] of the input data.
The tool is designed to be used in scripts and reads data from standard input and writes output into standard output.

Currently two algorithms are implemented: 
* Cumulative moving average
* Exponential moving average

## Building 
Project use make build system to build its from sources. It only has been tested on linux.

### Dependencies
- libeigen3

To install dependency please refer to your destribution manuanl.
* For Ubuntu users: 
```
> sudo apt-get install libeigen3-dev
```

* For Arch Linux users:
```
> sudo pacman -Sy eigen3 
```

To build the project simply use make:
```
 > make
```

 The resulting binary should be found in the same directory where the makefile is.


# Usage
Once build the tool is ready to be used via command line invocation or in scripts.

### Some what simple example:
```
> echo 1 2 3 4 5 5 5 5 5 5 5 3 2 1 | ./average
```

This will print intermidiate results for each input. 

By default the dimentionality of data is 1. That is it assumed that each input is a new datum point. 
It is also possible and sometimes usefull to compute average of the multi-dementional data. For example give following input file:

```
> cat data.txt 
0 0 0
1 0 0
1 2 0
1 2 1
1 2 1

```

Where each line is 3d vectod value, one can compute average like:

```
cat data.txt | ./average -d 3
1: { v: 0, max: [ 0, 0, 0 ], mean: [ 0, 0, 0 ], min: [ 0, 0, 0 ], }
2: { v: 0.707107, max: [ 1, 0, 0 ], mean: [ 0.5, 0, 0 ], min: [ 0, 0, 0 ], }
3: { v: 1.29099, max: [ 1, 2, 0 ], mean: [ 0.666667, 0.666667, 0 ], min: [ 0, 0, 0 ], }
4: { v: 1.35401, max: [ 1, 2, 1 ], mean: [ 0.75, 1, 0.25 ], min: [ 0, 0, 0 ], }
5: { v: 1.30384, max: [ 1, 2, 1 ], mean: [ 0.8, 1.2, 0.4 ], min: [ 0, 0, 0 ], }
```

Please note argument '-d' to the average command that specifiled dimentionality of the imput data.
With '-d 2' the same input will be interpreted as a list of 2d values.

### To compute exponential running average one should add '-e VALUE' or '--exp VALUE' argument
```
> echo 1 2 3 4 5 5 5 5 5 5 5 3 2 1 | ./average --exp 0.9
```

where VALUE is the 'waight of the recent observation' and not the waight of the accumulated summ (which is (1-VALUE)) as in some other sources.


### Getting summary
If you only interested in final value and not in the intermediate output you can add '-s' or '--summary' option:
```
> echo 1 2 3 4 5 5 5 5 5 5 5 3 2 1 | ./average --exp 0.9 --summary
14: { v: 1.59842, emp: [ 1.112 ], max: [ 5 ], mean: [ 3.64286 ], min: [ 0 ], }

```

## Contributing
This is trivial but hopefully usefull command line tool. If you find it usefull please fill free to contribute
