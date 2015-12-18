## Path list

`path_list.json` is a simple [JSON](https://en.wikipedia.org/wiki/JSON) file, which may consist of several paths, each has to have precisely three parts: `path` (string), `regex` (string) and `recursive` (boolean).

### Paths
Path is a simple string, which can be both absolute and relative. Note, though, that relative paths will be dealt from perspective of directory, from which daemon was started.

### Regular expression
Regexp is a simple string, too. Syntax used is [ECMAScript syntax](http://www.cplusplus.com/reference/regex/ECMAScript/), which is fairly close to Python *regex* library.

### Recursive
It is a flag, `true` or `false`, which shows if this configuration entry should go to nested directories or not.

### Example
You can find example of `path_list.json` in this repository. It covers all properties of configuration syntax.