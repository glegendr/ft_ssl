# ft_ssl
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

## Hash functions flags
- -p, --stdin &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- echo STDIN to STDOUT and append the checksum to STDOUT
- -q, --quiet &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- only print hash
- -r, --reverse &nbsp;&nbsp;&nbsp;&nbsp;- reverse the output
- -s, --string &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- the next argument is the string to hash
- -i, --input &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- input file  -optional-
- -o, --output &nbsp;&nbsp;&nbsp;&nbsp;- output file -default: stdout- -can be used once-
- -h, --help &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- get help on this function

## Cifers functions flags
### Bases flags
- -p, --stdin &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- echo STDIN to STDOUT and append the checksum to STDOUT
- -q, --quiet &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- only print hash
- -r, --reverse &nbsp;&nbsp;&nbsp;&nbsp;- reverse the output
- -s, --string &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- the next argument is the string to hash
- -d, --decrypt &nbsp;&nbsp;- decode hash
- -e, --encrypt &nbsp;&nbsp;- encode hash -optional-
- -i, --input &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- input file  -optional-
- -o, --output &nbsp;&nbsp;&nbsp;&nbsp;- output file -default: stdout- -can be used once-
- -h, --help &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- get help on this function

### Des flags
- -p, --password&nbsp;- the next argument is your pasword
- -k, --key &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- the next argument is your key in hexa
- -s, --salt &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- the next argument is your salt in hexa
- -v, --initvector&nbsp;&nbsp;- the next argument is your initialization vector -MANDATORY FOR DES, DES-CBC AND DES-PCBC-
- -a, --base64 &nbsp;&nbsp;&nbsp;&nbsp;- decode/encode the input/output in base64, depending on the encrypt mode
- &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;--string &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- the next argument is the string to hash
- -d, --decrypt &nbsp;&nbsp;&nbsp;- decode hash
- -e, --encrypt &nbsp;&nbsp;&nbsp;- encode hash -optional-
- -i, --input &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- input file  -optional-
- -o, --output &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- output file -default: stdout- -can be used once-
- -h, --help &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;- get help on this function
