# Integrity
### Project for first semester of 2015-2016

**Check integrity of your files.** This is a simple CLI application for storing and checking message digests of files on your hard drive. It is currently possible to find out and store hash sums of your files in database and then check it

## Usage
**Integrity** currently supports two modes of work:
#### Snapshot
To save file hash sum of file *filename* to database simply use:

```
integrity -s filename 
```

Currently, all hashsums are stored in file named *integrity.db*.

#### Check
To check if file was not corrupted use:

```
integrity -c filename
```

You will be returned with status of your current file: was it previously in database and was it corrupted.

## Compilation

To build this project you need *openssl* and *Berkeley DB* installed. Simply execute `build.sh` and the executable file `integrity` will be what you need. It uses *cmake* and *make* to build itself.