
## Usage

```
use BinarySearch;

my @vec = 1, 2, 3, 5, 6;
my Int $needle = 3;

my $res = binary-search @vec, $needle;

if $res != -1 {
	say "Position: $res" 
}
else {
	say "Value $needle not in array"
}

```


To run the program above (e.g., `main.raku`) in the same directory as
`lib` directory:

```
$ raku -Ilib main.raku
```
