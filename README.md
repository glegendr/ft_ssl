# ft_ssl
## Usage

ft_ssl hash_fct [-pqrde] [-s string] [-i in_file] [-o out_file]
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

## Allowed flags
- -p echo STDIN to STDOUT and append the checksum to STDOUT
- -q only print hash
- -r reverse the output
- -d decode hash -base64 only-
- -e encode hash -base64 only-
- -s hash the string
- -i input file  -optional-
- -o output file -default: stdout- -can only be used once-
