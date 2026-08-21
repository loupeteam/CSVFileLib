# Info
Library is provided by Loupe  
https://loupe.team  
info@loupe.team  
1-800-240-7042  

# Description
The CSVFileLib library provides a simple, intuitive interface for writing and reading variable values to and from comma separated value (CSV) files.

Files offer power fail safe data storage. They have the advantage over permanent memory that they are also protected against battery failure. Formatting files into a plain text CSV format allows them to be easily edited externally and then imported to the machine. The ability to open and save CSV files can be very useful for machine configuration and recipe storage, in particular.

Issues often arise during machine commissioning that are not apparent during development. Also, customer specific requirements can surface at any time during a machine’s lifecycle. Flexible machine configuration can provide solutions to these kinds of problems without requiring a machine software update or an engineer site visit.

The CSVFileLib library allows you to store machine configuration information in a plain text file that can be used to set the values of variables on the machine. And, since the file contains the names of the variables to be written, adding a new configuration item is as simple as adding a line of text to the file; nothing else needs to be modified, and no software updates are required.

The library is also robust to errors that might occur while handling CSV files. If a line in the file cannot be processed for any reason (for example, a mistyped variable name), that error is logged, and the next line is processed. The log information is available in a text file that can be useful in correcting the error.

It is also possible to save current variable values back to a CSV file, allowing you to implement machine configuration or recipe editing interfaces on the machine itself.

The library interface and file format were carefully designed to be as straight-forward and powerful as possible, allowing this functionality to be easily integrated into any project.

# Usage
The CSVFileLib functionality can be integrated into any project using a data structure and two function calls. HMI functionality requires another data structure and function call, as well as corresponding Visual Components elements.

For more documentation and examples, see https://loupeteam.github.io/LoupeDocs/libraries/csvfilelib.html

# Installation
To install using the Loupe Package Manager (LPM), in an initialized Automation Studio project directory run `lpm install csvfilelib`. For more information about LPM, see https://loupeteam.github.io/LoupeDocs/tools/lpm.html

# Tests

The example project carries a self checking test suite for the bit string
(BYTE/WORD/DWORD) support, in `example/AsProject/Logical/Programs/Default`.
It runs automatically a few scans after boot, and can be re-run by setting
`runTest`.

The `ARsim` configuration exists so the suite can actually be executed; the
`Intel` and `ARM` configurations target hardware. CI compiles all three, but
does not run the suite - executing it is a manual step.

## Running it

The suite reads two hand written input files, `crafted.csv` and `badline.csv`,
from the USER file device. They live in `example/UserPartition/ARsim/` and
have to be present on the device before the suite runs, otherwise the file
phases cannot complete.

Either copy them onto the USER file device by hand, or let a tool that reads
`example/AsProject/.loupe/partition.json` place them for you. That file is the
Loupe VS Code PLC Toolkit / bnr-build partition mapping - it is not something
LPM or the Automation Studio build does on its own.

## Reading the result

The suite writes `result.csv` to the USER file device.

**`suiteOk` is the verdict.** It is TRUE only when there were no failures, the
full expected number of assertions actually ran, and the suite reached the
end. Checking `testFail` alone is not enough: a file operation that never
completes leaves `testFail` at 0, which would otherwise read as a pass.

`firstFail` names the first case that failed, if any. The remaining values in
the file are there to make a failure diagnosable - the data types PV_ninfo
reported, and the values read back from each input file.

Note that the PV_ninfo data type codes were measured on ARsim running
Automation Runtime 6.7.6. The configurations in this project pin 6.6.2, which
is what CI compiles against.

## Licensing

This project is licensed under the [MIT License](LICENSE).