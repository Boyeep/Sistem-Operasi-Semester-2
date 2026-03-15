BEGIN {
    print "Student count by department"
}

{
    department[$3]++
}

END {
    for (name in department) {
        print name ":", department[name]
    }
}
