# Integrity
### Project for first semester of 2015-2016

**Check integrity of your files.** This is a simple CLI application for storing and checking message digests of files on your hard drive. It is currently possible to find out and store hash sums and other file metadata of your files in database and then check it.

## Usage
You should start daemon with `integrityctl --start`, and save file hashes manually with `integrityctl --store`, and then it will check your files automatically. If you want, you can force check all files with `integrityctl --check`.

You can find out complete command guide with `integrityctl --help`

## More
Help files on configuration and what files to check can be found in `config.md` and `path_list.md`

## Compilation

To build this project you need *Boost* libraries, *openssl* and *Berkeley DB* installed. This project also uses *plog* library, but it is bundled with the repository. Simply execute `build.sh` and the executable file `integrity` will be what you need. It uses *cmake* and *make* to build itself.

You will find executable at `bin/integrityctl`.