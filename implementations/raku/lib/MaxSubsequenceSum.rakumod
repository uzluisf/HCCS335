unit module MaxSubsequenceSum;

our sub v1( @a --> Int ) {
    my Int $max-sum = 0;
    for 0 ..^ @a.elems -> $i {
        for $i ..^ @a.elems -> $j {
            my Int $current-sum = 0;
            for $i .. $j -> $k {
                $current-sum += @a[$k]
            }
            if $current-sum > $max-sum {
                $max-sum = $current-sum
            }
        }
    }
    return $max-sum;
}

our sub v2( @a --> Int ) {
    my Int $max-sum = 0;
    for 0 ..^ @a.elems -> $i {
        my Int $current-sum = 0;
        for $i ..^ @a.elems -> $j {
            $current-sum += @a[$j];
            if $current-sum > $max-sum {
                $max-sum = $current-sum
            }
        }
    }
    return $max-sum;
}
