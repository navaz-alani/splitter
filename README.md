# splitter

[![CodeFactor](https://www.codefactor.io/repository/github/navaz-alani/splitter/badge)](https://www.codefactor.io/repository/github/navaz-alani/splitter)

`splitter` is a program which streamlines splitting of amounts amongst targets, even when there exist exemptions for different amounts.

Here is the motivating example: I have 4 'house-mates' and when we buy groceries, the bill needs to be split. This is always a tedious task because the total amount cannot be split evenly. Some people are exempt from the price of a certain item (e.g. vegetarians do not have to pay for the meat purchased). Furthermore, poor communication can lead to wrong calculations. For example, if someone forgot to mention that they would like to be exempt from an item, the calculations will need to be redone. Clearly, performing this task manually can waste a lot of precious time and energy.

So I created `splitter`; an abstract solution to the problem above. Firstly, the splitter syntax was created:

* The first line of every `.splitter` file has to be the declaration of the targets block. This is where targets are declared and single character symbols are assigned for easy reference.
  * The splitter syntax has 2 possible blocks: the targets block and the items block
  * The items block is where the items, with exemptions are listed
  * Targets and items blocks begin with `:targets` and `:items` respectively. A block should begin either on the first line of the document, or the line directly after the end of a previous block.
  * Targets and items blocks end with `:end-targets` and `:end-items` respectively.
* A target definition consists of 2 things: the name of the target and the corresponding symbol. 
  * There may be arbitrary whitespace before the first character of the definition
  * Whitespace after the first character will NOT be ignored (same for item definition)
  * The symbol used for the target will be the first character after the space following the target name.
  * In the target definition `personA a`, the target name will be `personA` and the corresponding symbol will be `a`. This is an equivalent definition: `personA abcde...`
* An item definition also consists of 2 parts: the amount and exemptions
  * There may be aribtrary whietspace before the first character of the definition
  * Whitespace after the first character will NOT be ignored
  * The exemptions for an item will be the string beginning at the first character after the space following the amount until the end of the line
  * The item definition `15.75 ae` will split the amount `15.75` amongst the remaining targets after exempting targets with symbols `a` and `e`.

After the targets and items blocks have been parsed, each target is printed (to stdout), along with the amount accumulated while splitting the items.

Secondly, a simple parser for the `splitter` syntax was created. The parser can be compiled by running `make build`, which will produce an output binary called `splitter`. The test file (`test.splitter`) can be interpreted by running `splitter tests/test.splitter`.

The file operations used in the source code are cross-platform compatible and therefore, there should not be many issues with the program.

Features such as being able to switch between exemptions and inclusions in the item definition are to be implemented. 

Collaboration is welcome (and appreciated) through GitHub.
