# Integrity
### Project for first semester of 2015-2016

**Check integrity of your files.** This is a simple CLI application for storing and checking message digests of files on your hard drive. It is currently possible to find out and store hash sums of your files in database and then check it

## Usage
```
General options:
  -h [ --help ]               produce this help message
  -s [ --store ]              save file hashes
  -c [ --check ]              check file hashes
  --config arg (=config.json) config file location
```

## Compilation

To build this project you need *Boost* libraries, *openssl* and *Berkeley DB* installed. Simply execute `build.sh` and the executable file `integrity` will be what you need. It uses *cmake* and *make* to build itself.