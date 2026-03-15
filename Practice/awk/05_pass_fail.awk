BEGIN {
    print "Pass/Fail Report"
    print "----------------"
}

{
    status = ($4 >= 75) ? "PASS" : "FAIL"
    print $2, $4, status
}
