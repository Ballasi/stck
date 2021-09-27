# stck

An esoteric programming language based around stacks

## Example

``` Hello world! : print ```

## How it works

Everytime you write a keyword, it will push to the stack. Keywords are
seperated by common separators such as spaces, backspaces, ...

For instance, with the example above, the first keyword is "Hello", the
second one is "world!", the third one is "." and the last one is "print".

You can also use spaces in keywords by using quotation marks, for instance,
the following code reproduces the same result as the example given.

``` "Hello world!" print ```

Each time a keyword is added to the stack, it will check if the given keyword
is a command. If so, it will execute the command and pop previous keywords.

This has a great potential of combining functions, as a function that returns
a value could be used to feed the arguments of another one. It reminds me
a bit of functional programming languages.

## Usage

A `Makefile` is available, just run `make` to launch the program if you're
on a Linux machine. If you want to build the binary, just run `make bin/stck`.

I haven't bothered working on Windows and MacOS support because that would
be most probably useless. I can do a Windows version if people are interested.

You can also run `stck /path/to/file.stck` if you have a `.stck` file you
want to run.

If you're using `stck` without any arguments, you will be redirect to the
interpreted. You can toy around with the language from now on!

I'd recommend you to take a look at the available commands, especially the
`stck` command as it may help you understand a bit more how it works.

## Available commands

Here is an exhaustive list of currently available commands

| Command | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| `stck`  | Prints the current stack to `stdout`                              |
| `.`     | Concatenates the last two elements of the stack                   |
| `..`    | Concatenates the last three elements of the stack                 |
| `:`     | Concatenates every elements of the stack                          |
| `print` | Prints to `stdout` the last element of the stack                  |
| `=`     | WIP, variable affectation                                         |

If you want to add an element to the stack that's a command without it running,
you can escape the command by using `\`. For instance, `\print`.

## Why though?

For fun, and it was partially inspired by the [reverse Polish
notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) that got
me wondered how that would transcribe in a programming language.

## License

Licensed under the GNU General Public License Version 3.0 ([LICENSE](LICENSE)
or https://www.gnu.org/licenses/gpl-3.0.html).
