## UID: 405938992

## Pipe Up

The program performs the pipe operator in shell - passing the output of one shell command as input to another shell command - allowing for the chaining of multiple commands together.

## Building

The program can be built on the CS111 VM on 64-bit Arch Linux using the command:
```sh
$ make
```

## Running

Show an example run of your program, using at least two additional arguments, and what to expect

The program is to be run as follows:
./pipe <command1> <command2> ... <commandN>

This is one possible example:
```sh
$ ./pipe ls cat wc
```

The output: 6   6   56

The program essentially uses the pipe operator like this: ls | cat | wc. The first command ls lists all current directory contents, and passes this output to cat. The cat command takes this as input and just writes the contents again, one per line, and further feeds it to wc. The wc command computes the word, line and character count respectively, and writes this to standard output.

When ran with just one command (./pipe <command1>), the program simply runs this command and writes to standard output.

## Cleaning up

Binary files can be cleaned up with the following shell command:
```sh
$ make clean
```