# pollard-rho-brent

Pollard Rho algorithm using Brent's implementation in C using the GMP library 
to find a prime factor given a number with OpenMPI for parallel processing. 
Seems to work ok on shorter semiprimes.(< T40)

Note: When compiling, specify the path to gmp.h using the -I flag 
if it is not automatically found by the compiler and make sure to compile 
with the -lgmp flag as well. E.g. gcc -I/usr/local/include/ main.cpp -lgmp
