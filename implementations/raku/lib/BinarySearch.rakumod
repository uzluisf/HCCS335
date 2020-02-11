unit module BinarySearch;

#| Return index of item $x in the sorted array @A. Otherwise,
#| return -1 if the item isn't in the array.
sub binary-search( @A, Int $x --> Int ) is export {
    my $low  = 0; 
    my $high = @A.elems - 1;
    while $low ≤ $high {
        my \mid = ($low + $high) div 2;  
        if    @A[mid] < $x { $low  = mid + 1 }
        elsif @A[mid] > $x { $high = mid - 1 }
        else               { return mid      }
    }
    return -1;
}

