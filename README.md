# ft_ssl
## Usage

ft_ssl hash_fct [-pqrdea] [-s string] [-i in_file] [-o out_file]
## Allowed hash functions and cifers functions
#### Hashs
- md5
- sha256
- sha512
- sha384
- sha224
#### Cifers
- base64
- base64url
- des-ecb
- des-cbc
- des-pcbc

## Generics flags
- -p echo STDIN to STDOUT and append the checksum to STDOUT -don't work with des functions-
- -q only print hash
- -r reverse the output
- -s hash the string -don't work with des functions-
- -i input file  -optional-
- -o output file -default: stdout- -can only be used once-

## Bases flags
- -d decode hash
- -e encode hash

## Des flags
- -d decode hash
- -e encode hash
- -p next arg is your password
- -k next arg is your key in hex
- -s next arg is your salt in hex
- -v next arg is your initialization vector in hex
- -a decode/encode the input/output in base64, depending on the encrypt mode
