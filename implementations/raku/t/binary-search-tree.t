use Test;
use BinarySearchTree;

subtest 'using nodes of integers', {
    my $bst = BinarySearchTree[Int].new(less-than => sub ($a, $b --> Bool) { $a < $b });

    is $bst.is-empty, True, 'BST is empty.';

    my @items = 6, 2, 8, 1, 4, 3;
    $bst.insert($_) for @items;

    is $bst.find-min, 1, 'find correct minimum value.';
    is $bst.find-max, 8, 'find correct maximum value.';

    subtest 'BST contains the right nodes.', {
        for @items {
            is $bst.contains($_), True, "BST contains node with value $_."
        }
    }

    for @items {
        $bst.remove($_) if $bst.contains($_)
    }

    is $bst.is-empty, True, 'BST is empty after removing all its nodes.';
}

subtest 'using nodes with objects', {
    class Person {
        has $.name;
        method Str { "Name: " ~ $!name }
    }

    my @names = <Alice Josiah Aerem Lem Dullias Xras>;
    my @items = gather for @names { take Person.new(:name($_)) };

    my $bst = BinarySearchTree[Person].new(
        less-than => sub ($a, $b --> Bool) { $a.name lt $b.name }
    );

    is $bst.is-empty, True, 'BST is empty.';

    $bst.insert($_) for @items;

    is $bst.find-min, @items.sort(*.name)[0], 'find correct minimum value.';
    is $bst.find-max, @items.sort(*.name)[*-1], 'find correct maximum value.';

    subtest 'BST contains the right nodes.', {
        for @items {
            is $bst.contains($_), True, "BST contains node with value $_."
        }
    }

    for @items {
        $bst.remove($_) if $bst.contains($_)
    }

    is $bst.is-empty, True, 'BST is empty after removing all its nodes.';
}
