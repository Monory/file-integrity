## Configuration

`path_list.json` is a simple [JSON](https://en.wikipedia.org/wiki/JSON) file, which may have these possible options:

- `sleep_duration` sets time in seconds between automatic file checks. If you put 0 in this field, scheduled file checks will not be happening
- `path_list_file` sets file with what files to check
- `log_severity` sets how verbose you want the logs. You have two possible configurations, `info` or `warning`
- `log_filename` sets in what file program will write logs

### Example
You can find example of `config.json` in this repository. It covers all properties of configuration syntax with default settings.
