# stck

An esoteric programming language based around stacks

## Example

``` Hello world! : print ```

## How it works

Everytime you write a keyword, it will push to the stack. Keywords are
seperated by common separators such as spaces, backspaces, ...

For instance, with the example above, the first keyword is "Hello", the
second one is "world!", the third one is ":" and the last one is "print".

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

If you're using `stck` without any arguments, you will be redirected to the
interpreter. You can toy around with the language from now on! See [CLI](#cli).

I would recommend you to take a look at the available commands, especially
the `stck` command as it may help you understand a bit more how it works.

It is also strongly recommended to check out [the examples](examples) to
get a grasp of how `stck` works. You can run examples using the command
`make examples/<name>.stck`.

### CLI

The command line interface is quite easy to use and looks like Python's. In
here, you can type instructions and it will be added to the stack. The
prompt either shows you `>` or `―`. The first one tells that your stack
is currently empty while the latter signifies your stack isn't. It can also
show you `┄` if you're currently in a comment.

If you want to take a look at your stack, you can type anytime the command
`stck` as it takes no argument. This command prints the stacks, from the bottom
(where your oldest keywords are) to the top (where the new keywords are).

I recommend playing around with the CLI if you're interested in understanding
a bit more how it works, it's a more interactive way of learning the basics
of stck!

## Available commands

Here is an exhaustive list of currently available commands

| Command | Description                                                       |
| ------- | ----------------------------------------------------------------- |
| `stck`  | Prints the current stack to `stdout`                              |
| `.`     | Concatenates the last two keywords of the stack                   |
| `..`    | Concatenates the last three keywords of the stack                 |
| `:`     | Concatenates every keywords of the stack                          |
| `print` | Prints to `stdout` the last element of the stack                  |
| `=`     | Variable affectation                                              |
| `~var`  | Push the value of the variable `var` to the stack                 |
| `/* */` | Comments                                                          |
| `\n`    | Prints a new line character                                       |

If you want to add an element to the stack that is a command without running
it, you can escape the command by using `\`. For instance, `\print`.

If a command cannot be launched, it will just push the content as is to
the stack (i.e. the command itself, not its result).

## Why though?

For fun, and it was partially inspired by the [reverse Polish
notation](https://en.wikipedia.org/wiki/Reverse_Polish_notation) that got
me wondered how that would transcribe in a programming language.

Also wanted to do a bit of C.

## License

Licensed under the GNU General Public License Version 3.0 ([LICENSE](LICENSE)
or https://www.gnu.org/licenses/gpl-3.0.html).
