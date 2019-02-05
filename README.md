# Nemo

Nemo is a command-line application for learning foreign languages.

## Building

``` shell
cmake . -Bbuild
cmake --build build
```

## Usage

After running Nemo, you will see several options. To choose one of them, type the digit preceding the given option. Then (if you have not chosen the `Exit` option), you will be prompted to enter the path to the wordlist. The path can be either relative or absolute. Below is a detailed description of each option.

### Test

Use this option to test your knowledge. You will be asked to choose ordering. The possibilities are:

1. In sequence
2. Randomly

The first option causes the data to appear in the order in which they appear in the file. The second is self-explanatory. Then, you will be prompted to translate the text to the left of the dash character. After providing an answer, you will be informed if it was correct or not. If not, the correct translation will be displayed additionally. You can stop the test at any time by pressing <kbd>Ctrl</kbd>+<kbd>D</kbd> on Unix or <kbd>Ctrl</kbd>+<kbd>Z</kbd> followed by <kbd>Enter</kbd> on Windows. After the test, the percentage of correct answers will be displayed.

### Create new wordlist

To create new wordlist choose this option, specify the path to it, and enter the data line by line. After completing the data entry, press <kbd>Ctrl</kbd>+<kbd>D</kbd> on Unix or <kbd>Ctrl</kbd>+<kbd>Z</kbd> followed by <kbd>Enter</kbd> on Windows to save the wordlist and back to menu. Each line should have the following format:

```
text-translation1;translation2;...;translationN
```

You can also use an external text editor for this task.

### Modify wordlist

If you select this option and enter the correct path to the file, the wordlist with line numbering followed by a few options will be displayed. The options are:

1. Modify lines
2. Erase lines
3. Add lines
4. Return

After choosing one of the first two, you will be prompted to enter the range (right-open interval) in the following form: `begin-end`. The first option replaces the lines in the given range to those given by the user. The third one appends lines at the end of the wordlist.

### Show wordlist

This option shows the wordlist with line numbering.

### Delete wordlist

After specifying the path, confirm that you want to delete the file by entering `y` or cancel by entering `n`. In the first case, the given file will be deleted.

### Exit

Select this option to exit the program.

## License

[MIT](https://github.com/wadiim/nemo/blob/master/LICENSE)
