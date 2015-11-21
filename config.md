## Configuration

`config.json` is a simple [JSON](https://en.wikipedia.org/wiki/JSON) file, which may consist of several blocks, each has to has precisely two parts: `paths` and `regex`.

### Paths
Paths are just simple JSON array. The only important part is that paths that end in **forward slashes** will be processed recursively, otherwise only not nested files will be treated.

### Regular expressions
Regexps are simple JSON array, too. Syntax used is [ECMAScript syntax](http://www.cplusplus.com/reference/regex/ECMAScript/), which is fairly close to Python *regex* library.

### Example
You can find example of `config.json` in this repository. It covers all properties of configuration syntax.