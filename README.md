# SEIRD - Coupled Neighborhoods

This instructions assume that:

1. Cadmium has been installed following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

## Compile the model

1. Install Cadmium following the instructions in the manual:
http://www.sce.carleton.ca/courses/sysc-5104/lib/exe/fetch.php?media=cadmiumusermanual.pdf

2. Compile the model using the make file

## Run the model

### Default parameters (Defcault parameters run using input_data_vanier.txt and input_data_rockcliffe.txt)

> ./bin/SEIRD (linux)

> ./bin/SEIRD.exe (windows)
 
**NOTE**
1. The program requires that the data to be in a specific order.
To ensure this requirement is satisfied, we provide a template.txt file where the user can add the parameters

When inputting data values please do not add additional whitespace. 
One is fine but the program will not recognise data after that.
Examples

> mortality = 10.3  //okay

> mortality =10.3   //okay

> mortality =  10.3 //NOT okay 

2. Ensure all the data is inputted, otherwise the program will not run and it will display an appropriate error message.
It will only display the first missing data tag it encounters and any subsequent tags will not be read