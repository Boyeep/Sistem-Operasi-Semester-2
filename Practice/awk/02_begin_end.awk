BEGIN {
    print "Student Report"
    print "--------------"
}

{
    print $2, "from", $3, "has score", $4
}

END {
    print "--------------"
    print "End of report"
}
