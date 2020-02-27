unit role HashTable[::T];

use Adverb::Eject;

has $.hash is required where { .arity == 1 && .returns ~~ Int };

has Array $!lists = Array[Array].new(shape => 19);
has Int   $!num-of-items;

method contains( T $x --> Bool ) {
    $x ∈ $!lists[ self!hashify($x) ]
}

method make-empty( --> Nil ) {
    $!list = Empty; 
}

method insert( T $x --> Bool ) {
    if $!lists[ self!hashify($x) ] {
        return False if $x ∈ $!lists[ self!hashify($x) ]
    }

    $!lists[ self!hashify($x) ].push($x);

    self!rehash if (++$!num-of-elements > @!lists.elems);
    return True;
}

method remove( T $x --> Bool ) {
    if $!lists[ self!hashify($x) ] {
        return False unless $x ∈ $!lists[ self!hashify($x) ]
    }

    $!lists[ self!hashify($x) ]:eject;
    $!num-of-items -= 1; 
    return True;
}


method !hashify( T $x --> Int ) {
    try {
        return $!hash($x) mod $!lists.elems;
    }

    if $! {
        note "Error: Couldn't apply hash function to argument '{$x.gist}' of type {T.raku}.";
        note "Did you mean to define the hash function as\n" ~
             ('    sub ( ', T.raku, ' $x ) { }', "\n?").join;
    }
}

method !rehash {
    sub next-prime(UInt $n is copy) {
        $n++ if $n %% 2;
        $n += 2 until $n.is-prime;
        return $n
    }

    my @old-lists = $!lists;

    # create new double-sized, empty table.
    $!lists = Empty;
    $!lists = Array[Array].new(shape => next-prime 2 * $!lists.elems);

    # copy old table into new table.
    $!num-of-items = 0;
    for $!lists -> $list {
        for $list -> $x {
            self.insert($x)
        }
    }
}
