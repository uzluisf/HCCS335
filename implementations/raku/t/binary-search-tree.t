use Test;
use BinarySearchTree;

subtest 'using nodes of integers', {
    my $bst = BinarySearchTree[Int].new;

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

subtest 'using nodes with Point2D objects', {
    class Point2D is Cool {
        has Int:D $.x = 0;
        has Int:D $.y = 0;

        multi method distance( --> Numeric ) {
            self.distance: self, Point2D.new(:0x, :0y);
        }

        multi method distance( Point2D:D $A, Point2D:D $B --> Numeric ) {
            (($A.x - $B.x)**2 + ($A.y - $B.y)**2).sqrt
        }

        method gist {
            "Point2D($!x, $!y)"
        }

        method Numeric {
            self.distance
        }
    }

    my @points = 
        Point2D.new(:3x, :4y), Point2D.new(:6x, :8y), Point2D.new(:1x, :1y),
        Point2D.new(:5x, :8y), Point2D.new(:15x, :2y), Point2D.new(:20x, :10y),
    ;

    my $bst = BinarySearchTree[Point2D].new;

    is $bst.is-empty, True, 'BST is empty.';

    $bst.insert($_) for @points;

    cmp-ok $bst.find-min, &[===], @points.sort(+*)[0], 'find correct minimum value.';

    cmp-ok $bst.find-max, &[===], @points.sort(+*)[*-1], 'find correct maximum value.';

    subtest 'BST contains the right nodes.', {
        for @points {
            is $bst.contains($_), True, "BST contains node with value {$_.gist}."
        }
    }

    for @points {
        $bst.remove($_) if $bst.contains($_)
    }

    is $bst.is-empty, True, 'BST is empty after removing all its nodes.';
}
