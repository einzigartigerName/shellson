# shellson
This is/will be a simple implementation of an Unix-Shell written in C.\
Only capable of executing single commands.\
Just doing it for the fun and the experience.
## Features
* Command execution
* Options for prompt

#### Prompt
A config file called `.shellsonrc` will be created in your home directiory (`/home/username/`).\
There you can modify the `PROMPT` variable, default will be this:
```
PROMPT = "%u@%h$"
```
* `%u` will be replaced with your username
* `%h` will be replaced with the host name of your maschine
* `%%` will be your normal percent sign

## Test it - Use at own risk
```
git clone https://github.com/einzigartigername/shellson
cd shellson/
make
```
You will get two warnigs about unused parameters `argc` and `argv` in `main`, just ignore them, they do not matter.

To start simply execute: `path/to/shellson`\
or if you have installed it:`shellson`.
### Install
if you are crazy and plant to use it frequenttly, you can install it via
```
make install
```

### Uninstall
```
make uninstall
```
