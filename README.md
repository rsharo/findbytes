# findbytes
Finds part of one binary file inside another

## Prereqs (Debian package names)
   * gcc
   * make
   * libboost-dev
   * libboost-iostreams-dev
   * libboost-program-options-dev
   
## Build instructions
```
make
```

## How to use
```
$ findbytes
Allowed options:
  --help                           produce help message
  --pf arg                         pattern file name
  --tf arg                         file to search
  --ps arg (=0)                    offset into pattern file to start
  --pe arg (=18446744073709551615) offset into pattern file to end
  --ts arg (=0)                    offset into target file to begin search
  --te arg (=18446744073709551615) offset into target file to end search
  --verbose                        describe what is happening
```

Example:
```
$ echo "foo" > foo.txt
$ echo "12341234ooxyzzy" > tgt.txt
$ findbytes foo.txt tgt.txt
$ findbytes foo.txt tgt.txt --ps 1
$ findbytes foo.txt tgt.txt --ps 1 --pe 3
8
$ echo -n "foo" > foo.txt
$ findbytes foo.txt tgt.txt --ps 1
8
```

### Credits
Most credit goes to [dubnde's stackexchange posting](http://unix.stackexchange.com/questions/39728/how-to-find-offset-of-one-binary-file-inside-another#39739)
