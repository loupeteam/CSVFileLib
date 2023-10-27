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

## Licensing

This project is licensed under the [MIT License](LICENSE.md).