BEGIN {
    total = 0
    count = 0
    print "Students with score >= 80"
}

$4 >= 80 {
    print $2, $4
    total += $4
    count++
}

END {
    if (count > 0) {
        print "Average:", total / count
    } else {
        print "No data"
    }
}
