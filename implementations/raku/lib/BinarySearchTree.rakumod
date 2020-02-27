unit role BinarySearchTree[::T];

my role BinaryNode[::T] {
    has T $.item           is rw;
    has BinaryNode $.left  is rw;
    has BinaryNode $.right is rw;
}

has BinaryNode $!root;

=comment PUBLIC METHODS

#| Find smallest item in the BST.
method find-min( BinarySearchTree:D: ) {
    self!rec-find-min($!root).item
}

#| Find largest item in the BST.
method find-max( BinarySearchTree:D: ) {
    self!iter-find-max($!root).item
}

#| Check if BST contains item $x.
method contains( BinarySearchTree:D: T $x --> Bool ) {
    self!rec-contains($x, $!root)
}

#| Check if BST is empty.
method is-empty( BinarySearchTree:D: --> Bool ) {
    !$!root.defined
}

#| Insert item $x into the BST.
method insert( BinarySearchTree:D: T $x --> Nil ) {
    #die "Please implement the Real method 
    self!rec-insert($x, $!root)
}

#| Remove item $x from the BST.
method remove( BinarySearchTree:D: T $x --> Nil ) {
    self!rec-remove($x, $!root)
}

=comment PRIVATE METHODS

method !rec-find-min( BinaryNode $node --> BinaryNode ) {
    return BinaryNode if not $node.defined;
    # current node's left subtree doesn't exist
    return $node if not $node.left.defined;
    # go deeper to the left
    return self!rec-find-min($node.left)
}

method !iter-find-max( BinaryNode $node is copy --> BinaryNode ) {
    if $node.defined {
        # keep going right while current node's right subtree is defined
        $node = $node.right while $node.right.defined;
    }
    return $node;
}

method !rec-contains( T $x, BinaryNode $node is rw --> Bool ) {
    return False unless $node.defined;

    if $x < $node.item {
        self!rec-contains($x, $node.left)
    }
    elsif $node.item < $x {
        self!rec-contains($x, $node.right)
    }
    else {
        return True
    }
}

method !rec-insert( T $x, BinaryNode $node is rw --> Nil ) {
    if !$node.defined {
        $node = BinaryNode[$(T)].new(item => $x);
    }
    elsif $x < $node.item {
        self!rec-insert($x, $node.left)
    }
    elsif $node.item < $x {
        self!rec-insert($x, $node.right)
    }
    else {
        # Duplicate; do nothing
    }
}

method !rec-remove( T $x, BinaryNode $node is rw --> Nil ) {
    return unless $node.defined;

    # go left
    if $x < $node.item {
        self!rec-remove($x, $node.left)
    }
    # go right
    elsif $node.item < $x {
        self!rec-remove($x, $node.right)
    }
    # node with two children
    elsif $node.left.defined and $node.right.defined {
        # find current node's right subtree's smallest data.
        my $item = self!rec-find-min($node.right).item;
        # replace current node's data with smallest data.
        $node.item = $item;
        # remove node with smallest data from right subtree.
        self!rec-remove($item, $node.right);
    }
    # node with less than two single children
    else {
        $node = $node.left.defined ?? $node.left !! $node.right;
    }
}

