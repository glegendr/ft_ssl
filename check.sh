make -j;
if [[ $1 = "md5" || $1 = "" ]]; then
echo "-------------------------------------------------------------------------"
echo "                                  md5                                    "
echo "-------------------------------------------------------------------------"
echo ""
./ft_ssl md5 -h
echo ""
echo "salut" | ./ft_ssl md5
echo "salut" | md5
echo ""
echo "0123456789abcdef" | ./ft_ssl md5 -pqr
echo "0123456789abcdef" | openssl md5
echo ""
./ft_ssl md5 -q test/test1
md5 test/test1
echo ""
./ft_ssl md5 -rq test/test2
md5 test/test2
echo ""
./ft_ssl md5 -r test/test3
md5 test/test3
echo ""
echo "salut" | ./ft_ssl md5 src/main.c
echo "salut" | md5 src/main.c
echo ""
echo "salut" | ./ft_ssl md5 makefile
openssl md5  makefile
echo ""
echo "salut" | ./ft_ssl md5 test/void_test -vdk 0123456798abcdef
openssl md5 test/void_test
echo ""
fi
if [[ $1 = "sha" || $1 = "" || $1 = "sha256" || $1 = "sha512" || $1 = "sha224" || $1 = "sha384" ]]; then
echo "-------------------------------------------------------------------------"
echo "                                  sha                                    "
echo "-------------------------------------------------------------------------"
echo ""
./ft_ssl sha256 -h
echo ""
if [[ $1 = "sha" || $1 = "" || $1 = "sha256" ]]; then
echo "salut" | ./ft_ssl sha256
echo "salut" | openssl sha -sha256
echo ""
echo "0123456789abcdef" | ./ft_ssl sha256 -pqr
echo "0123456789abcdef" | openssl sha -sha256
echo ""
./ft_ssl sha256 -q test/test1
openssl sha -sha256 test/test1
echo ""
./ft_ssl sha256 -rq test/test2
openssl sha -sha256 test/test2
echo ""
./ft_ssl sha256 -r test/test3
openssl sha -sha256 test/test3
echo ""
echo "salut" | ./ft_ssl sha256 src/main.c
echo "salut" | openssl sha -sha256 src/main.c
echo ""
echo "salut" | ./ft_ssl sha256 makefile
openssl sha -sha256  makefile
echo ""
echo "salut" | ./ft_ssl sha256 test/void_test -vdk 0123456798abcdef
openssl sha -sha256 test/void_test
fi
if [[ $1 = "sha" || $1 = "" || $1 = "sha224" ]]; then
echo "---------------------------------\nSHA224"
echo ""
echo "salut" | ./ft_ssl sha224
echo "salut" | openssl sha -sha224
echo ""
echo "0123456789abcdef" | ./ft_ssl sha224 -pqr
echo "0123456789abcdef" | openssl sha -sha224
echo ""
./ft_ssl sha224 -q test/test1
openssl sha -sha224 test/test1
echo ""
./ft_ssl sha224 -rq test/test2
openssl sha -sha224 test/test2
echo ""
./ft_ssl sha224 -r test/test3
openssl sha -sha224 test/test3
echo ""
echo "salut" | ./ft_ssl sha224 src/main.c
echo "salut" | openssl sha -sha224 src/main.c
echo ""
echo "salut" | ./ft_ssl sha224 makefile
openssl sha -sha224  makefile
echo ""
echo "salut" | ./ft_ssl sha224 test/void_test -vdk 0123456798abcdef
openssl sha -sha224 test/void_test
echo ""
fi
if [[ $1 = "sha" || $1 = "" || $1 = "sha512" ]]; then
echo "---------------------------------\nsha512"
echo ""
echo "salut" | ./ft_ssl sha512
echo "salut" | openssl sha -sha512
echo ""
echo "0123456789abcdef" | ./ft_ssl sha512 -pqr
echo "0123456789abcdef" | openssl sha -sha512
echo ""
./ft_ssl sha512 -q test/test1
openssl sha -sha512 test/test1
echo ""
./ft_ssl sha512 -rq test/test2
openssl sha -sha512 test/test2
echo ""
./ft_ssl sha512 -r test/test3
openssl sha -sha512 test/test3
echo ""
echo "salut" | ./ft_ssl sha512 src/main.c
echo "salut" | openssl sha -sha512 src/main.c
echo ""
echo "salut" | ./ft_ssl sha512 makefile
openssl sha -sha512  makefile
echo ""
echo "salut" | ./ft_ssl sha512 test/void_test -vdk 0123456798abcdef
openssl sha -sha512 test/void_test
echo ""
fi
if [[ $1 = "sha" || $1 = "" || $1 = "sha384" ]]; then
echo "---------------------------------\nsha384"
echo ""
echo "salut" | ./ft_ssl sha384
echo "salut" | openssl sha -sha384
echo ""
echo "0123456789abcdef" | ./ft_ssl sha384 -pqr
echo "0123456789abcdef" | openssl sha -sha384
echo ""
./ft_ssl sha384 -q test/test1
openssl sha -sha384 test/test1
echo ""
./ft_ssl sha384 -rq test/test2
openssl sha -sha384 test/test2
echo ""
./ft_ssl sha384 -r test/test3
openssl sha -sha384 test/test3
echo ""
./ft_ssl sha384 src/main.c
echo "salut" | openssl sha -sha384 src/main.c
echo ""
echo "salut" | ./ft_ssl sha384 makefile
openssl sha -sha384  makefile
echo ""
echo "salut" | ./ft_ssl sha384 test/void_test -vdk 0123456798abcdef
openssl sha -sha384 test/void_test
echo ""
fi
fi
if [[ $1 = "bases" || $1 = "" ]]; then
echo "-------------------------------------------------------------------------"
echo "                                 bases                                   "
echo "-------------------------------------------------------------------------"
echo ""
./ft_ssl base64 -h
echo ""
echo "salut" | ./ft_ssl base64
echo "salut" | openssl  base64
echo ""
echo "0123456789abcdef" | ./ft_ssl base64 -pqr
echo "0123456789abcdef" | openssl  base64
echo ""
./ft_ssl base64 -q test/test1
openssl base64 -in test/test1
echo ""
./ft_ssl base64 -rq test/test2
openssl base64 -in test/test2
echo ""
./ft_ssl base64 -r test/test3
openssl base64 -in test/test3
echo ""
./ft_ssl base64 src/main.c
echo "salut" | openssl base64 -in src/main.c
echo ""
echo "salut" | ./ft_ssl base64 makefile
openssl base64  -in makefile
echo ""
echo "salut" | ./ft_ssl base64url test/void_test -vdk 0123456798abcdef
openssl base64 -in test/void_test
echo ""
fi
if [[ $1 = "des" || $1 = "" ]]; then
echo "-------------------------------------------------------------------------"
echo "                                  des                                    "
echo "-------------------------------------------------------------------------"
echo "\nHASH TEST3"
./ft_ssl des-ecb -ia test/test3 -k 0123456789abcdef -v 0123456789abcdef;
openssl des-ecb -in test/test3 -K 0123456789abcdef -iv 0123456789abcdef -a;
echo "---------------------------------\nHASH TEST1"
./ft_ssl des-ecb -ia test/test1 -k 0123456789abcdef;
openssl des-ecb -in test/test1 -K 0123456789abcdef -a;
echo "";
./ft_ssl des-cbc  -a -k 6162636461626364 -v 0011223344556677 -i test/test1;
openssl des-cbc  -a -K 6162636461626364 -iv 0011223344556677 -in test/test1
echo "---------------------------------\nHASH VOIDTEST"
./ft_ssl des-cbc -ia test/void_test -k fedcba9876543210 -v 0123456789abcdef;
openssl des-cbc -in test/void_test -K fedcba9876543210 -iv 0123456789abcdef -a;
echo "---------------------------------\nUNHASH TEST3"
echo "tP0jFkelvsAK2DhDmyoG0A=="| ./ft_ssl des-ecb -ad  -k 0123456789abcdef -v 0123456789abcdef | cat -e;
echo "tP0jFkelvsAK2DhDmyoG0A=="| openssl des-ecb -a -d  -K 0123456789abcdef -iv 0123456789abcdef | cat -e;
echo "---------------------------------\nUNHASH TEST1"
echo "IPrpPRzZ30zeUPqIZCUyQAhvmh10yU1O" | ./ft_ssl des-ecb -da -k 0123456789abcdef | cat -e;
echo ""
echo "IPrpPRzZ30zeUPqIZCUyQAhvmh10yU1O" | openssl des-ecb -d -a -K 0123456789abcdef | cat -e;
echo ""
echo ""
echo "bFEJlZhbWaGJ1VNWa+fJRZm8KnrUnuy5" | ./ft_ssl des-cbc  -ad -k 6162636461626364 -v 0011223344556677;
echo ""
echo "bFEJlZhbWaGJ1VNWa+fJRZm8KnrUnuy5" | openssl des-cbc  -a -d -K 6162636461626364 -iv 0011223344556677;
echo "\n---------------------------------\nUNHASH VOIDTEST"
echo "/dcRNm4JIfc=" | ./ft_ssl des-cbc -da -k fedcba9876543210 -v 0123456789abcdef | cat -e;
echo "/dcRNm4JIfc=" | openssl des-cbc -d -a -K fedcba9876543210 -iv 0123456789abcdef | cat -e;
echo "--------> HASH in cbc \"salut les potes comment vont les vacances aujurd'hui ?\""
echo "salut les potes comment vont les vacances aujurd'hui ?" | ./ft_ssl des-cbc  -k fedcba9876543210 -v 0123456789abcdef -a;
echo "";
echo "salut les potes comment vont les vacances aujurd'hui ?" | openssl des-cbc  -K fedcba9876543210 -iv 0123456789abcdef -a;
echo "\n--------> HASH the result in ecb"
echo "ot3oK3gchG6VcSeq0qt09jhJ1FG+/hjFGJhJAFFWbdwFyXa6wdvD/mPsJ9vRPosU6pEkL34P95I=" | ./ft_ssl des-ecb -s 9784615320bafced -p "coucou les potes" -a
echo "";
echo "ot3oK3gchG6VcSeq0qt09jhJ1FG+/hjFGJhJAFFWbdwFyXa6wdvD/mPsJ9vRPosU6pEkL34P95I=" | openssl des-ecb -S 9784615320bafced -pass "pass:coucou les potes" -a
echo "\n--------> Then unhash 2 times"
echo "U2FsdGVkX1+XhGFTILr87VQSuI0QKjV6mAsiLJGFhmR+omDQ+vl3bSrsPR6tWsbh
Yjkn6pJvdhJVAHaEv8wCy2w6fyy4ErHNTTHjhOUhQfpxx5Fok2jfjeXRUt5AAW/x" | ./ft_ssl des-ecb -s 9784615320bafced -pd "coucou les potes" -a;
echo "ot3oK3gchG6VcSeq0qt09jhJ1FG+/hjFGJhJAFFWbdwFyXa6wdvD/mPsJ9vRPosU6pEkL34P95I=" |./ft_ssl des-cbc -da -k fedcba9876543210 -v 0123456789abcdef;
echo ""
echo ""
echo "U2FsdGVkX1+XhGFTILr87VQSuI0QKjV6mAsiLJGFhmR+omDQ+vl3bSrsPR6tWsbh
Yjkn6pJvdhJVAHaEv8wCy2w6fyy4ErHNTTHjhOUhQfpxx5Fok2jfjeXRUt5AAW/x" | openssl des-ecb -S 9784615320bafced -pass "pass:coucou les potes" -d -a;
echo "ot3oK3gchG6VcSeq0qt09jhJ1FG+/hjFGJhJAFFWbdwFyXa6wdvD/mPsJ9vRPosU
6pEkL34P95I=" | openssl des-cbc -d -a -K fedcba9876543210 -iv 0123456789abcdef;
fi
